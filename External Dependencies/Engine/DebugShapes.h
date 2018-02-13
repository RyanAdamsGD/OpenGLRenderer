#ifndef DEBUG_SHAPES
#define DEBUG_SHAPES

#define USING_DEBUG_SHAPES

#include <vector>
#include "ExportHeader.h"
#include "RenderManager.h"
#include "glm\gtx\transform.hpp"
#include "TransformInfo.h"

class ENGINE_SHARED DebugShapes
{
	DebugShapes();
	DebugShapes(const DebugShapes&);
	DebugShapes& operator =(const DebugShapes&);

	struct DebugRenderable{
		uint renderableID;
		TransformInfo transform;
		float duration;
	};
	static const uint MAX_NUM_DEBUGRENDERABLE =100;
	DebugRenderable shapes[MAX_NUM_DEBUGRENDERABLE];
	uint nextDebugRenderable;

	struct DebugUpdatingBox{
		uint debugShapeID;
		const TransformInfo* xfm;
		DebugUpdatingBox(): xfm(NULL){}
	};
	DebugUpdatingBox updatingBoxes[MAX_NUM_DEBUGRENDERABLE];
	uint nextUpdatingBox;

	uint arrowGeometryID;
	uint lineGeometryID;
	uint sphereGeometryID;
	uint circleGeometryID;
	uint triangleGeometryID;
	uint wfCubeGeometryID;
	uint cubeID;
	uint whiteTexture,redTexture,blueTexture,greenTexture;

	void loadShapeData();
	void setUpTextures();
	uint findNextIndex();

	static DebugShapes instance;

public:
	//Adds a box that moves based on the position passed in
	void AddUpdatingBox(TransformInfo* xfm,
		float duration =3.0f, 
		bool depthEnabled = true);
	// Adds a line segment to the debug drawing queue.
	void AddArrow( const glm::vec3& fromPosition,
		const glm::vec3& toPosition,
		float lineWidth = 1.0f,
		float duration = 3.0f,
		bool depthEnabled = true);
	// Adds a line segment to the debug drawing queue.
	void AddLine( const glm::vec3& fromPosition,
		const glm::vec3& toPosition,
		float lineWidth = 1.0f,
		float duration = 3.0f,
		bool depthEnabled = true);
	// Adds an axis-aligned cross (3 lines converging at
	// a point) to the debug drawing queue.
	void AddCross( const glm::vec3& position,
		float size,
		float duration = 3.0f,
		bool depthEnabled = true);
	// Adds a wireframe sphere to the debug drawing queue.
	void AddSphere( const glm::vec3& centerPosition,
		float radius,
		float duration = 3.0f,
		bool depthEnabled = true);
	// Adds a circle to the debug drawing queue.
	void AddCircle( const glm::vec3& centerPosition,
		const glm::vec3& planeNormal,
		float radius,
		float duration = 3.0f,
		bool depthEnabled = true);

	// Adds a set of coordinate axes depicting the
	// position and orientation of the given
	// transformation to the debug drawing queue.
	void AddAxes( const glm::mat4& xfm,
		float size,
		float duration = 3.0f,
		bool depthEnabled = true);
	// Adds an axis-aligned bounding box to the debug
	// queue.
	void AddAABB( const glm::vec3& minCoords,
		const glm::vec3& maxCoords,
		float lineWidth = 1.0f,
		float duration = 3.0f,
		bool depthEnabled = true);
	// Adds an oriented bounding box to the debug queue.
	void AddOBB( const glm::mat4& centerTransform,
		const glm::vec3& scaleXYZ,
		float lineWidth = 1.0f,
		float duration = 3.0f,
		bool depthEnabled = true);

	void update(float dt);
	void drawText();
	void init();

	static DebugShapes& getInstance() { return instance; }
};

#define Debug_Shapes DebugShapes::getInstance()

#endif