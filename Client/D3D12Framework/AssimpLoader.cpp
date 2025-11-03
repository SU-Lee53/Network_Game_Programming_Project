#include "pch.h"
#include "AssimpLoader.h"

std::string strTexturePath = "../Resource/";

AssimpLoader::AssimpLoader()
{
	m_pImporter = std::make_shared<Assimp::Importer>();
}

std::pair<MESHLOADINFO, MATERIALLOADINFO> AssimpLoader::LoadMesh(aiMesh* rpMesh)
{
	MESHLOADINFO meshLoadInfo{}; 
	
	// Vertices
	UINT nVertices = rpMesh->mNumVertices;
	for (int i = 0; i < nVertices; ++i) {
		if (rpMesh->HasPositions()) {
			meshLoadInfo.v3Positions.push_back(Vector3(rpMesh->mVertices[i].x, rpMesh->mVertices[i].y, rpMesh->mVertices[i].z));
		}
		if (rpMesh->HasNormals()) {
			meshLoadInfo.v3Normals.push_back(Vector3(rpMesh->mNormals[i].x, rpMesh->mNormals[i].y, rpMesh->mNormals[i].z));
		}
		if (rpMesh->HasTangentsAndBitangents()) {
			meshLoadInfo.v3Tangents.push_back(Vector3(rpMesh->mTangents[i].x, rpMesh->mTangents[i].y, rpMesh->mTangents[i].z));
		}
		if (rpMesh->GetNumUVChannels() > 0) {
			meshLoadInfo.v2TexCoord0.push_back(Vector2(rpMesh->mTextureCoords[0][i].x, rpMesh->mTextureCoords[0][i].y));
		}

	}

	// 나중에 확인 필요
	aiAABB aiBound = rpMesh->mAABB;
	Vector3 v3Center((aiBound.mMax.x + aiBound.mMin.x) / 2, (aiBound.mMax.y + aiBound.mMin.y) / 2, (aiBound.mMax.z + aiBound.mMin.z) / 2);
	Vector3 v3Extents(std::fabs(v3Center.x - aiBound.mMax.x), std::fabs(v3Center.y - aiBound.mMax.y), std::fabs(v3Center.z - aiBound.mMax.z));
	meshLoadInfo.xmf3AABBCenter = v3Center;
	meshLoadInfo.xmf3AABBExtents = v3Extents;

	// Indices
	UINT nFaces = rpMesh->mNumFaces;
	meshLoadInfo.nIndices.resize(1);
	meshLoadInfo.nIndices[0] = rpMesh->mNumFaces * 3; // Assimp 가 인덱스가 아니고 면(Face) 갯수를 저장함ㅋㅋ

	meshLoadInfo.SubMeshes.resize(1);
	for (int i = 0; i < nFaces; ++i) {
		for (int j = 0; j < rpMesh->mFaces[i].mNumIndices; ++j) {
			meshLoadInfo.SubMeshes[0].push_back(rpMesh->mFaces[i].mIndices[j]);
		}
	}

	// 참고로 Assimp 는 SubMesh 개념이 없고 Material 이 다르면 Mesh 를 짤라버림
	// 그러니까 Mesh 당 Material 은 하나만 나옴
	MATERIALLOADINFO matLoadInfo = LoadMaterials(m_rpScene->mMaterials[rpMesh->mMaterialIndex]);

	return { meshLoadInfo, matLoadInfo };
}

MATERIALLOADINFO AssimpLoader::LoadMaterials(aiMaterial* rpMaterial)
{
	MATERIALLOADINFO materialLoadInfo;

	aiColor4D aicValue{};
	if (rpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aicValue) == AI_SUCCESS) {
		materialLoadInfo.v4Diffuse = XMFLOAT4(&aicValue.r);
	}

	if (rpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, aicValue) == AI_SUCCESS) {
		materialLoadInfo.v4Ambient = XMFLOAT4(&aicValue.r);
	}

	if (rpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, aicValue) == AI_SUCCESS) {
		materialLoadInfo.v4Specular = XMFLOAT4(&aicValue.r);
	}

	if (rpMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, aicValue) == AI_SUCCESS) {
		materialLoadInfo.v4Emissive = XMFLOAT4(&aicValue.r);
	}

	float fValue{};
	if (rpMaterial->Get(AI_MATKEY_SHININESS, fValue) == AI_SUCCESS) {
		materialLoadInfo.fGlossiness = fValue;
	}

	if (rpMaterial->Get(AI_MATKEY_ROUGHNESS_FACTOR, fValue) == AI_SUCCESS) {
		materialLoadInfo.fSmoothness = 1 - fValue;
	}

	if (rpMaterial->Get(AI_MATKEY_METALLIC_FACTOR, fValue) == AI_SUCCESS) {
		materialLoadInfo.fMetallic = fValue;
	}

	if (rpMaterial->Get(AI_MATKEY_REFLECTIVITY, fValue) == AI_SUCCESS) {
		materialLoadInfo.fGlossyReflection = fValue;
	}

	aiString aistrTexturePath{};

	if (rpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aistrTexturePath) == AI_SUCCESS) {
		materialLoadInfo.strAlbedoMapName = strTexturePath + ExtractFilename(aistrTexturePath.C_Str());
	}

	if (rpMaterial->GetTexture(aiTextureType_SPECULAR, 0, &aistrTexturePath) == AI_SUCCESS) {
		materialLoadInfo.strSpecularMapName = strTexturePath + ExtractFilename(aistrTexturePath.C_Str());
	}

	if (rpMaterial->GetTexture(aiTextureType_METALNESS, 0, &aistrTexturePath) == AI_SUCCESS) {
		materialLoadInfo.strMetallicMapName = strTexturePath + ExtractFilename(aistrTexturePath.C_Str());
	}

	if (rpMaterial->GetTexture(aiTextureType_NORMALS, 0, &aistrTexturePath) == AI_SUCCESS) {
		materialLoadInfo.strNormalMapName = strTexturePath + ExtractFilename(aistrTexturePath.C_Str());
	}

	if (rpMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &aistrTexturePath) == AI_SUCCESS) {
		materialLoadInfo.strEmissionMapName = strTexturePath + ExtractFilename(aistrTexturePath.C_Str());
	}

	if (rpMaterial->GetTexture(aiTextureType_DIFFUSE, 1, &aistrTexturePath) == AI_SUCCESS) {
		materialLoadInfo.strDetailAlbedoMapName = strTexturePath + ExtractFilename(aistrTexturePath.C_Str());
	}

	if (rpMaterial->GetTexture(aiTextureType_NORMALS, 1, &aistrTexturePath) == AI_SUCCESS) {
		materialLoadInfo.strDetailNormalMapName = strTexturePath + ExtractFilename(aistrTexturePath.C_Str());
	}

	return materialLoadInfo;
}

std::string AssimpLoader::ExtractFilename(const char* strPath)
{
	return std::filesystem::path{ strPath }.filename().string();
}
