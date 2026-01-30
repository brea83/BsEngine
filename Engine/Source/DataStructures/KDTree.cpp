#include "BsPrecompileHeader.h"
#include "KDTree.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

namespace Pixie
{
	namespace Data
	{
		KDTreeVec3::KDTreeVec3(std::vector<glm::vec3> points)
		{
			std::vector<uint32_t> indices;
			for (uint32_t i = 0; i < points.size(); i++)
			{
				indices.push_back(i);
			}

			Root = CreateNode(points, indices, 0, 0, points.size() - 1);
		}

		KDTreeVec3::~KDTreeVec3()
		{
			delete Root;
		}

		KDNode* KDTreeVec3::CreateNode(std::vector<glm::vec3>& points, std::vector<uint32_t>& indices, uint32_t iDimension, int32_t iStart, int32_t iEnd)
		{
			if (iStart > iEnd) return nullptr;

			Sort(points, indices, iDimension, iStart, iEnd);

			// select median
			uint32_t iMiddle = (iStart + iEnd) / 2;
			glm::vec3 median = points[iMiddle];

			// calc next dimension
			uint32_t iNextDimension = (iDimension + 1) % 3;

			KDNode* node = new KDNode(indices[iMiddle], median, nullptr, nullptr);
			node->LeftChild = CreateNode(points, indices, iNextDimension, iStart, iMiddle - 1);
			node->RightChild = CreateNode(points, indices, iNextDimension, iMiddle + 1, iEnd);

			return node;
		}

		uint32_t KDTreeVec3::Partition(std::vector<glm::vec3>& points, std::vector<uint32_t>& indices, uint32_t iDimension, int32_t iStart, int32_t iEnd)
		{
			//select pivot value
			glm::vec3 pivot = points[(iStart + iEnd) / 2];
			int32_t iLeft = iStart;
			int32_t iRight = iEnd;

			//test to make sure I understand glm vecs
			glm::vec3 number{ 0.0f, 1.0f, 2.0f };
			float index = number[2];

			while (iLeft <= iRight)
			{
				// move left index unti a value greater or equal to pivot is found
				while (points[iLeft][iDimension] < pivot[iDimension])
				{
					iLeft++;
				}

				// move right index unti a value less than or equal to the pivot is found
				while (points[iRight][iDimension] > pivot[iDimension])
				{
					iRight--;
				}

				//check if we should swap
				if (iLeft <= iRight)
				{
					glm::vec3 temp = points[iLeft];
					points[iLeft] = points[iRight];
					points[iRight] = temp;

					uint32_t tempIndex = indices[iLeft];
					indices[iLeft] = indices[iRight];
					indices[iRight] = tempIndex;

					iLeft++;
					iRight--;
				}
			}

			return iLeft;
		}

		void KDTreeVec3::Sort(std::vector<glm::vec3>& points, std::vector<uint32_t>& indices, uint32_t iDimension, int32_t iStart, int32_t iEnd)
		{
			if (iStart >= iEnd) return;

			uint32_t pivotIndex = Partition(points, indices, iDimension, iStart, iEnd);

			//sort left
			Sort(points, indices, iDimension, iStart, pivotIndex - 1);

			//sort right
			Sort(points, indices, iDimension, pivotIndex, iEnd);
		}

		KDSearchData KDTreeVec3::GetNextSearchData(glm::vec3& value, KDNode* node, uint32_t iDimension)
		{
			// if we should search the left half, next branch is left child, and other banch is right child
			bool bShouldSearchLeft = value[iDimension] < node->Value[iDimension];
			KDNode* nextBranch = bShouldSearchLeft ? node->LeftChild : node->RightChild;
			KDNode* otherBranch = bShouldSearchLeft ? node->RightChild : node->LeftChild;

			// calculate next dimension
			uint32_t iNextDimension = (iDimension + 1) % 3;
			return { nextBranch, otherBranch, iNextDimension };
		}

		KDNode* KDTreeVec3::FindNearestNeighbor(KDNode* node, glm::vec3 value, uint32_t iDimension, std::vector<KDNode*>&visitedNodes)
		{
			if (node == nullptr) return nullptr;

			visitedNodes.push_back(node);

			//get next search
			KDSearchData data = GetNextSearchData(value, node, iDimension);

			KDNode* temp = FindNearestNeighbor(data.NextBranch, value, data.NextDimension, visitedNodes);
			KDNode* best = GetClosest(value, temp, node);

			// should we search the other branch?
			float distanceToBest = glm::distance2(value, best->Value);
			float distanceToPlane = glm::abs(value[iDimension] - node->Value[iDimension]);
			//comparing to squared distance so square this dist
			distanceToPlane *= distanceToPlane;
			
			if (distanceToBest >= distanceToPlane)
			{
				temp = FindNearestNeighbor(data.OtherBranch, value, data.NextDimension, visitedNodes);
				best = GetClosest(value, temp, best);
			}

			return best;
		}

		KDNode* KDTreeVec3::GetClosest(glm::vec3& value, KDNode* A, KDNode* B)
		{
			float distanceA = A != nullptr ? glm::distance2(value, A->Value) : FLT_MAX;
			float distanceB = B != nullptr ? glm::distance2(value, B->Value) : FLT_MAX;
			return distanceA < distanceB ? A : B;
		}

		std::vector<KDNode*> KDTreeVec3::FindNodesInRange(glm::vec3 value, float fRange)
		{
			std::vector<KDNode*> nodesInRange;
			FindNodesInRange(Root, value, fRange, 0, nodesInRange);
			return nodesInRange;
		}

		void KDTreeVec3::FindNodesInRange(KDNode* node, glm::vec3 value, float fRange, uint32_t iDimension, std::vector<KDNode*>& nodesInRange)
		{
			if (node == nullptr) return;

			float squaredDistance = glm::distance2(value, node->Value);

			if (squaredDistance <= fRange * fRange)
			{
				nodesInRange.push_back(node);
			}

			//begin search of children
			KDSearchData data = GetNextSearchData(value, node, iDimension);

			// search next branch
			FindNodesInRange(data.NextBranch, value, fRange, data.NextDimension, nodesInRange);
		}
	}
}