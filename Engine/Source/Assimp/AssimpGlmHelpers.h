#pragma once

#include<assimp/quaternion.h>
#include<assimp/vector3.h>
#include<assimp/matrix4x4.h>
#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>


class AssimpGlmHelpers
{
public:

	static inline glm::mat4 ConvertMatrixToGlmFormat(const aiMatrix4x4& assimpMatrix)
	{
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = assimpMatrix.a1; to[1][0] = assimpMatrix.a2; to[2][0] = assimpMatrix.a3; to[3][0] = assimpMatrix.a4;
		to[0][1] = assimpMatrix.b1; to[1][1] = assimpMatrix.b2; to[2][1] = assimpMatrix.b3; to[3][1] = assimpMatrix.b4;
		to[0][2] = assimpMatrix.c1; to[1][2] = assimpMatrix.c2; to[2][2] = assimpMatrix.c3; to[3][2] = assimpMatrix.c4;
		to[0][3] = assimpMatrix.d1; to[1][3] = assimpMatrix.d2; to[2][3] = assimpMatrix.d3; to[3][3] = assimpMatrix.d4;
		return to;
	}

	static inline glm::vec3 GetGlmVec(const aiVector3D& assimpVec3)
	{
		return glm::vec3(assimpVec3.x, assimpVec3.y, assimpVec3.z);
	}

	static inline glm::quat GetGlmQuat(const aiQuaternion& assimpOrientation)
	{
		return glm::quat(assimpOrientation.w, assimpOrientation.x, assimpOrientation.y, assimpOrientation.z);
	}
};