#pragma once
#include "glm/glm.hpp"

namespace Pixie
{
	namespace Data
	{
		template <typename Type>
		struct Tree
		{
			using value_type = Type;

			struct Node
			{
				Type Value;
				std::vector<Node> Children;
			};

			Node Root;
		};


		template <typename Type>
		struct BinaryTree
		{
			using value_type = Type;

			struct Node
			{
				Type Value;
				Node* LeftChild{ nullptr };
				Node* RightChild{ nullptr };
			};

			Node* Root{ nullptr };

			int Depth()
			{
				return CalculateDepth(Root, 0);
			}

			int CalculateDepth(Node* node, int iDepth)
			{
				if (node == nullptr)
				{
					return iDepth;
				}

				int newDepth = std::max(CalculateDepth(node->LeftChild, iDepth + 1),
					CalculateDepth(node->RightChild, iDepth + 1));
				return newDepth;
			}

			static bool IsNodeBalanced(Node* node)
			{
				return std::abs(CalculateDepth(node->LeftChild, 0) - CalculateDepth(node->RightChild, 0)) <= 1;
			}

			static int GetBalance(Node* node)
			{
				if (node == nullptr) return 0;

				return CalculateDepth(node->LeftChild, 0) - CalculateDepth(node->RightChild, 0);
			}

		};

	}
}


