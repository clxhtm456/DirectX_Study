#include "Framework.h"
#include "Terrain.h"

Terrain::Terrain(Shader * shader, wstring heightFile)
	: Renderer(shader)
{
	sBaseMap = shader->AsSRV("BaseMap");
	sAlphaMap = shader->AsSRV("AlphaMap");
	sLayerMap = shader->AsSRV("LayerMap");

	heightMap = new Texture(heightFile);

	brushBuffer = new ConstantBuffer(&brushDesc, sizeof(BrushDesc));
	sBrushBuffer = shader->AsConstantBuffer("CB_Brush");

	lineBuffer = new ConstantBuffer(&lineDesc, sizeof(LineDesc));
	sLineBuffer = shader->AsConstantBuffer("CB_TerrainLine");

	CreateVertexData();
	CreateIndexData();
	CreateNormalData();
	
	vertexBuffer = new VertexBuffer
		(vertices, vertexCount, sizeof(TerrainVertex), 0, true);
	indexBuffer = new IndexBuffer(indices, indexCount);
}

Terrain::~Terrain()
{
	SafeDelete(heightMap);

	SafeDeleteArray(vertices);
	SafeDelete(vertexBuffer);

	SafeDeleteArray(indices);
	SafeDelete(indexBuffer);

	SafeDelete(brushBuffer);
	SafeDelete(lineBuffer);

	SafeDelete(baseMap);
	SafeDelete(layerMap);
	SafeDelete(alphaMap);
}

void Terrain::Update()
{	
	Super::Update();	

	if (brushDesc.Type > 0)
	{
		brushDesc.Location = GetPickedPosition();

		if (Mouse::Get()->Press(0))
			RaiseHeight(brushDesc.Location, brushDesc.Type, brushDesc.Range);
	}
	
}

void Terrain::Render()
{
	Super::Render();

	if(baseMap != NULL)
		sBaseMap->SetResource(baseMap->SRV());	

	brushBuffer->Apply();
	sBrushBuffer->SetConstantBuffer(brushBuffer->Buffer());

	lineBuffer->Apply();
	sLineBuffer->SetConstantBuffer(lineBuffer->Buffer());

	if (layerMap != NULL && alphaMap != NULL)
	{
		sAlphaMap->SetResource(alphaMap->SRV());
		sLayerMap->SetResource(layerMap->SRV());
	}
	
	
	shader->DrawIndexed(0, Pass(), indexCount);

}

void Terrain::BaseMap(wstring file)
{
	SafeDelete(baseMap);
	baseMap = new Texture(file);
}

void Terrain::LayerMap(wstring file, wstring alpha)
{
	SafeDelete(alphaMap);
	SafeDelete(layerMap);

	alphaMap = new Texture(alpha);
	layerMap = new Texture(file);
}

float Terrain::GetHeight(Vector3& position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + (x + 1);
	index[3] = width * (z + 1) + (x + 1);

	Vector3 v[4];
	for (int i = 0; i < 4; i++)
		v[i] = vertices[index[i]].Position;

	float ddx = (position.x - v[0].x) / 1.0f;
	float ddz = (position.z - v[0].z) / 1.0f;

	Vector3 result;

	if(ddx + ddz <= 1)
		result = v[0] + (v[2] - v[0]) * ddx + (v[1] - v[0]) * ddz;
	else
	{
		ddx = 1 - ddx;
		ddz = 1 - ddz;
		result = v[3] + (v[1] - v[3]) * ddx + (v[2] - v[3]) * ddz;
	}

	return result.y;

}

float Terrain::GetHeightPick(Vector3 & position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + (x + 1);
	index[3] = width * (z + 1) + (x + 1);

	Vector3 p[4];
	for (int i = 0; i < 4; i++)
		p[i] = vertices[index[i]].Position;

	Vector3 start(position.x, 1000, position.z);
	Vector3 direction(0, -1, 0);

	float u, v, distance;
	Vector3 result(-1, FLT_MIN, -1);

	
	if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
		result = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;

	if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
		result = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;

	return result.y;
}

Vector3 Terrain::GetPickedPosition()
{
	Matrix V = Context::Get()->View();
	Matrix P = Context::Get()->Projection();
	Viewport* vp = Context::Get()->GetViewport();

	Vector3 mouse = Mouse::Get()->GetPosition();

	Vector3 n, f;

	mouse.z = 0.0f;
	vp->UnProject(&n, mouse, transform->World(), V, P);

	mouse.z = 1.0f;
	vp->UnProject(&f, mouse, transform->World(), V, P);

	Vector3 start = n;
	Vector3 direction = f - n;

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{			
			UINT index[4];
			index[0] = width * z + x;
			index[1] = width * (z + 1) + x;
			index[2] = width * z + (x + 1);
			index[3] = width * (z + 1) + (x + 1);

			Vector3 p[4];
			for (int i = 0; i < 4; i++)
				p[i] = vertices[index[i]].Position;

			float u, v, distance;
			if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
				return p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;

			if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
				return p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
		}
	}

	return Vector3(-1, FLT_MIN, -1);
}

void Terrain::CreateVertexData()
{
	//heightMap의 색상 정보 받아오기
	vector<Color> heights;
	heightMap->ReadPixel(DXGI_FORMAT_R8G8B8A8_UNORM, &heights);

	width = heightMap->GetWidth();
	height = heightMap->GetHeight();

	//Create VData
	vertexCount = width * height;
	vertices = new TerrainVertex[vertexCount];

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index = width * z + x;
			UINT pixel = width * (height - z - 1) + x;

			vertices[index].Position.x = (float)x;
			vertices[index].Position.y = (heights[pixel].r * 256.0f) / 10.0f;
			vertices[index].Position.z = (float)z;

			vertices[index].Uv.x = (float)x / (float)width;
			vertices[index].Uv.y = (float)(height - 1 - z) / (float)height;
		}
	}
}

void Terrain::CreateIndexData()
{
	indexCount = (width - 1) * (height - 1) * 6;
	indices = new UINT[indexCount];

	UINT index = 0;
	for (UINT y = 0; y < height - 1; y++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices[index + 0] = width * y + x;
			indices[index + 1] = width * (y + 1) + x;
			indices[index + 2] = width * y + (x + 1);
			indices[index + 3] = width * y + (x + 1);
			indices[index + 4] = width * (y + 1) + x;
			indices[index + 5] = width * (y + 1) + (x + 1);

			index += 6;
		}
	}
}

void Terrain::CreateNormalData()
{
	for (UINT i = 0; i < indexCount / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		TerrainVertex v0 = vertices[index0];
		TerrainVertex v1 = vertices[index1];
		TerrainVertex v2 = vertices[index2];

		Vector3 d1 = v1.Position - v0.Position;
		Vector3 d2 = v2.Position - v0.Position;

		Vector3 normal;
		D3DXVec3Cross(&normal, &d1, &d2);
		
		vertices[index0].Normal = normal;
		vertices[index1].Normal = normal;
		vertices[index2].Normal = normal;		
	}

	for (UINT i = 0; i < vertexCount; i++)
		D3DXVec3Normalize(&vertices[i].Normal, &vertices[i].Normal);
}

void Terrain::RaiseHeight(Vector3 & position, UINT type, UINT range)
{
	D3D11_BOX rect;
	rect.left = (LONG)position.x - range;
	rect.top = (LONG)position.z + range;
	rect.right = (LONG)position.x + range;
	rect.bottom = (LONG)position.z - range;

	if (rect.left < 0) rect.left = 0;
	if (rect.right >= width) rect.right = width;
	if (rect.bottom < 0) rect.bottom = 0;
	if (rect.top >= height) rect.top = height;

	for (LONG z = rect.bottom; z <= rect.top; z++)
	{
		for (LONG x = rect.left; x <= rect.right; x++)
		{
			UINT index = width * (UINT)z + (UINT)x;
			vertices[index].Position.y += 5.0f * Time::Delta();
		}
	}

	CreateNormalData();

	/*D3D::GetDC()->UpdateSubresource
	(
		vertexBuffer->Buffer(), 0, NULL, vertices, sizeof(TerrainVertex) * vertexCount, 0
	);*/
	
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(TerrainVertex) * vertexCount);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
}

///과제
// 1. BrushType -> Sphere Type 추가
// 2. Shift키를 누르고 클릭하면 지면 하강
// 3. Noise(랜덤)
// 4. Smooth(평균화)
// 5. Flat(평탄화)
// 6. Slope(두 점사이의 경사)