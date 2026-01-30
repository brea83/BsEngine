#pragma once
#include "Trees.h"

namespace Pixie
{
	namespace Data
	{
		struct KDNode
		{
			KDNode() = default;
			KDNode(uint32_t index, glm::vec3 value, KDNode* left, KDNode* right)
				: Index(index), Value(value), LeftChild(left), RightChild(right)
			{}

			~KDNode()
			{
				delete LeftChild;
				delete RightChild;
			}

			//index is to save the original index position in the list used to make this kdTree
			uint32_t Index{ 0 };
			glm::vec3 Value{ glm::vec3(0.0f) };
			KDNode* LeftChild{ nullptr };
			KDNode* RightChild{ nullptr };
		};

		struct KDSearchData
		{
			KDSearchData() = default;
			KDSearchData(KDSearchData&) = default;

			KDNode* NextBranch{ nullptr };
			KDNode* OtherBranch{ nullptr };
			uint32_t NextDimension{ 0 };
		};

		struct KDTreeVec3
		{
			
			KDTreeVec3(std::vector<glm::vec3> points);
			~KDTreeVec3();
			// properties
			KDNode* Root{ nullptr };

			// functions
			KDNode* CreateNode(std::vector<glm::vec3>& points, std::vector<uint32_t>& indices, uint32_t iDimension, int32_t iStart, int32_t iEnd);

			uint32_t Partition(std::vector<glm::vec3>& points, std::vector<uint32_t>& indices, uint32_t iDimension, int32_t iStart, int32_t iEnd);
			void Sort(std::vector<glm::vec3>& points, std::vector<uint32_t>& indices, uint32_t iDimension, int32_t iStart, int32_t iEnd);

			KDSearchData GetNextSearchData(glm::vec3& value, KDNode* node, uint32_t iDimension);

			KDNode* FindNearestNeighbor(glm::vec3 value)
			{
				std::vector<KDNode*> visitedNodes;
				return FindNearestNeighbor(Root, value, 0, visitedNodes);
			}
			KDNode* FindNearestNeighbor(KDNode* node, glm::vec3 value, uint32_t iDimension, std::vector<KDNode*>& visitedNodes);

			KDNode* GetClosest(glm::vec3& value, KDNode* A, KDNode* B);

			std::vector<KDNode*> FindNodesInRange(glm::vec3 value, float fRange);
			void FindNodesInRange(KDNode* node, glm::vec3 value, float fRange, uint32_t iDimension, std::vector<KDNode*>& nodesInRange);

		};

	}
}

