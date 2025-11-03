#pragma once


class AssimpLoader {
public:
	AssimpLoader();

public:
	template<typename objType, typename meshRendererType>
	requires std::derived_from<objType, GameObject> && std::derived_from<meshRendererType, MeshRenderer>
	std::shared_ptr<objType> Load(const std::string& strPath);


private:
	template<typename meshRendererType>
		requires std::derived_from<meshRendererType, MeshRenderer>
	std::shared_ptr<meshRendererType> LoadMeshRenderer(aiNode* rpNode);

	template<typename objType, typename meshRendererType>
		requires std::derived_from<objType, GameObject>&& std::derived_from<meshRendererType, MeshRenderer>
	std::shared_ptr<objType> LoadObject(aiNode* rpNode, std::shared_ptr<GameObject> pParent = nullptr);

	std::pair<MESHLOADINFO, MATERIALLOADINFO> LoadMesh(aiMesh* rpMesh);
	MATERIALLOADINFO LoadMaterials(aiMaterial* rpMesh);

	std::string ExtractFilename(const char* strPath);

private:
	std::string m_strPath;

	std::shared_ptr<Assimp::Importer> m_pImporter = nullptr;
	const aiScene* m_rpScene = nullptr;
	aiNode* m_rpRootNode = nullptr;
	UINT m_nNodes = 0;

};

template<typename objType, typename meshRendererType>
	requires std::derived_from<objType, GameObject> && std::derived_from<meshRendererType, MeshRenderer>
inline std::shared_ptr<objType> AssimpLoader::Load(const std::string& strPath)
{
	m_rpScene = m_pImporter->ReadFile(
		strPath,
		aiProcess_MakeLeftHanded |
		aiProcess_FlipUVs |
		aiProcess_FlipWindingOrder |	// Convert to D3D
		//aiProcess_PreTransformVertices | 
		aiProcess_JoinIdenticalVertices |
		aiProcess_Triangulate |
		aiProcess_GenUVCoords |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_PopulateArmatureData
	);

	if (!m_rpScene) {
		__debugbreak();
	}

	m_rpRootNode = m_rpScene->mRootNode;

	std::shared_ptr<objType> pObj = LoadObject<objType, meshRendererType>(m_rpRootNode);

	return pObj;
}

template<typename meshRendererType>
	requires std::derived_from<meshRendererType, MeshRenderer>
inline std::shared_ptr<meshRendererType> AssimpLoader::LoadMeshRenderer(aiNode* rpNode)
{
	std::vector<std::shared_ptr<meshRendererType>> pMeshRenderers;
	pMeshRenderers.reserve(rpNode->mNumMeshes);
	for (UINT i = 0; i < rpNode->mNumMeshes; ++i) {
		auto [mesh, material] = LoadMesh(m_rpScene->mMeshes[rpNode->mMeshes[i]]);
		std::shared_ptr<meshRendererType>pMeshRenderer = std::make_shared<meshRendererType>(mesh, std::vector{ material });
		pMeshRenderers.push_back(pMeshRenderer);
	}

	assert(pMeshRenderers.size() <= 1);

	if (pMeshRenderers.size() == 0) {
		return nullptr;
	}
	return pMeshRenderers[0];
}

template<typename objType, typename meshRendererType>
	requires std::derived_from<objType, GameObject>&& std::derived_from<meshRendererType, MeshRenderer>
inline std::shared_ptr<objType> AssimpLoader::LoadObject(aiNode* rpNode, std::shared_ptr<GameObject> pParent)
{
	std::shared_ptr<objType> pObj = std::make_shared<objType>();
	pObj->SetMeshRenderer(LoadMeshRenderer<meshRendererType>(rpNode));
	pObj->SetParent(pParent);
	pObj->SetFrameName(rpNode->mName.C_Str());
	aiMatrix4x4 aimtxFrame = rpNode->mTransformation.Transpose();
	pObj->GetTransform().SetFrameMatrix(Matrix(&aimtxFrame.a1));
	
	for (int i = 0; i < rpNode->mNumChildren; ++i) {
		pObj->SetChild(LoadObject<GameObject, meshRendererType>(rpNode->mChildren[i], pObj));
	}

	return pObj;
}
