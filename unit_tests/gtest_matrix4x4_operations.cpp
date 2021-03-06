#include "gtest_matrix4x4.h"

namespace {

class Matrix4x4OperationsTest : public ::testing::Test
{
  public:
	Matrix4x4OperationsTest()
	    : m1_(nc::Matrix4x4f::rotationX(60))
	{}

	nc::Matrix4x4f m1_;
};

TEST_F(Matrix4x4OperationsTest, MultiplyVector4Right)
{
	printMatrix("m1:\n", m1_);

	const nc::Vector4f v1 = nc::Vector4f(0.5f, 1.0f, 0.75f, 1.0f).normalize();
	printf("v1: <%.2f, %.2f, %.2f, %.2f> (length: %.2f)\n", v1.x, v1.y, v1.z, v1.w, v1.length());
	const nc::Vector4f tr = m1_ * v1;
	printf("Multiplying the matrix (left) by the vector (right): <%.2f, %.2f, %.2f, %.2f> (length: %.2f)\n", tr.x, tr.y, tr.z, tr.w, tr.length());

	ASSERT_FLOAT_EQ(v1.length(), 1.0f);
	ASSERT_FLOAT_EQ(tr.length(), 1.0f);
	ASSERT_FLOAT_EQ(tr.x, nc::dot(m1_[0], v1));
	ASSERT_FLOAT_EQ(tr.y, nc::dot(m1_[1], v1));
	ASSERT_FLOAT_EQ(tr.z, nc::dot(m1_[2], v1));
	ASSERT_FLOAT_EQ(tr.w, nc::dot(m1_[3], v1));
}

TEST_F(Matrix4x4OperationsTest, MultilpyVector3Right)
{
	printMatrix("m1:\n", m1_);

	const nc::Vector3f v1 = nc::Vector3f(0.5f, 1.0f, 0.75f).normalize();
	printf("v1: <%.2f, %.2f, %.2f> (length: %.2f)\n", v1.x, v1.y, v1.z, v1.length());
	const nc::Vector3f tr = m1_ * v1;
	printf("Multiplying the matrix (left) by the vector (right): <%.2f, %.2f, %.2f> (length: %.2f)\n", tr.x, tr.y, tr.z, tr.length());

	ASSERT_FLOAT_EQ(v1.length(), 1.0f);
	ASSERT_FLOAT_EQ(tr.length(), 1.0f);
	ASSERT_FLOAT_EQ(tr.x, nc::dot(m1_[0].toVector3(), v1));
	ASSERT_FLOAT_EQ(tr.y, nc::dot(m1_[1].toVector3(), v1));
	ASSERT_FLOAT_EQ(tr.z, nc::dot(m1_[2].toVector3(), v1));
}

TEST_F(Matrix4x4OperationsTest, MultiplyVector4LeftTranslate)
{
	const float x = 10.0f;
	const float y = 15.0f;
	const float z = 5.0f;
	m1_ = nc::Matrix4x4f::translation(-x, -y, -z);
	printMatrix("m1:\n", m1_);

	const nc::Vector4f v1 = nc::Vector4f(x, y, z, 1.0f);
	printf("v1: <%.2f, %.2f, %.2f, %.2f>\n", v1.x, v1.y, v1.z, v1.w);
	const nc::Vector4f tr = v1 * m1_;
	printf("Multiplying the vector (left) by the matrix (right): <%.2f, %.2f, %.2f, %.2f>\n", tr.x, tr.y, tr.z, tr.w);

	ASSERT_FLOAT_EQ(tr.x, 0.0f);
	ASSERT_FLOAT_EQ(tr.y, 0.0f);
	ASSERT_FLOAT_EQ(tr.z, 0.0f);
	ASSERT_FLOAT_EQ(tr.w, 1.0f);
}

TEST_F(Matrix4x4OperationsTest, MultiplyVector4LeftRotate)
{
	const float x = 10.0f;
	m1_ = nc::Matrix4x4f::rotationZ(90.0f);
	printMatrix("m1:\n", m1_);

	const nc::Vector4f v1 = nc::Vector4f(x, 0.0f, 0.0f, 1.0f);
	printf("v1: <%.2f, %.2f, %.2f, %.2f>\n", v1.x, v1.y, v1.z, v1.w);
	const nc::Vector4f tr = v1 * m1_;
	printf("Multiplying the vector (left) by the matrix (right): <%.2f, %.2f, %.2f, %.2f>\n", tr.x, tr.y, tr.z, tr.w);

	ASSERT_FLOAT_EQ(v1.length(), tr.length());
	ASSERT_NEAR(tr.x, 0.0f, 1e-6f);
	ASSERT_FLOAT_EQ(tr.y, x);
	ASSERT_FLOAT_EQ(tr.z, 0.0f);
	ASSERT_FLOAT_EQ(tr.w, 1.0f);
}

TEST_F(Matrix4x4OperationsTest, MultiplyVector3LeftRotate)
{
	const float x = 10.0f;
	m1_ = nc::Matrix4x4f::rotationZ(90.0f);
	printMatrix("m1:\n", m1_);

	const nc::Vector3f v1 = nc::Vector3f(x, 0.0, 0.0f);
	printf("v1: <%.2f, %.2f, %.2f>\n", v1.x, v1.y, v1.z);
	const nc::Vector3f tr = v1 * m1_;
	printf("Multiplying the vector (left) by the matrix (right): <%.2f, %.2f, %.2f>\n", tr.x, tr.y, tr.z);

	ASSERT_FLOAT_EQ(v1.length(), tr.length());
	ASSERT_NEAR(tr.x, 0.0f, 1e-6f);
	ASSERT_FLOAT_EQ(tr.y, x);
	ASSERT_FLOAT_EQ(tr.z, 0.0f);
}

TEST_F(Matrix4x4OperationsTest, Transposed)
{
	printMatrix("m1:\n", m1_);
	const nc::Matrix4x4f tr = m1_.transposed();
	printMatrix("Transposing the matrix:\n", tr);

	assertVectorsAreEqual(tr[0], m1_[0].x, m1_[1].x, m1_[2].x, m1_[3].x);
	assertVectorsAreEqual(tr[1], m1_[0].y, m1_[1].y, m1_[2].y, m1_[3].y);
	assertVectorsAreEqual(tr[2], m1_[0].z, m1_[1].z, m1_[2].z, m1_[3].z);
	assertVectorsAreEqual(tr[3], m1_[0].w, m1_[1].w, m1_[2].w, m1_[3].w);
}

TEST_F(Matrix4x4OperationsTest, TransposeInPlace)
{
	const nc::Matrix4x4f oldM1 = m1_;

	printMatrix("m1:\n", m1_);
	m1_.transpose();
	printMatrix("Transposing the matrix in place:\n", m1_);

	assertVectorsAreEqual(m1_[0], oldM1[0].x, oldM1[1].x, oldM1[2].x, oldM1[3].x);
	assertVectorsAreEqual(m1_[1], oldM1[0].y, oldM1[1].y, oldM1[2].y, oldM1[3].y);
	assertVectorsAreEqual(m1_[2], oldM1[0].z, oldM1[1].z, oldM1[2].z, oldM1[3].z);
	assertVectorsAreEqual(m1_[3], oldM1[0].w, oldM1[1].w, oldM1[2].w, oldM1[3].w);
}

TEST_F(Matrix4x4OperationsTest, InvertIdentity)
{
	printMatrix("The identity matrix:\n", nc::Matrix4x4f::Identity);
	const nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity.inverse();
	printMatrix("Inverting the identity matrix:\n", newMatrix);

	assertVectorsAreEqual(newMatrix[0], nc::Matrix4x4f::Identity[0]);
	assertVectorsAreEqual(newMatrix[1], nc::Matrix4x4f::Identity[1]);
	assertVectorsAreEqual(newMatrix[2], nc::Matrix4x4f::Identity[2]);
	assertVectorsAreEqual(newMatrix[3], nc::Matrix4x4f::Identity[3]);
}

TEST_F(Matrix4x4OperationsTest, InvertOrthogonal)
{
	printMatrix("m1 is orthogonal:\n", m1_);
	const nc::Matrix4x4f tr = m1_.transposed();
	printMatrix("Creating a new matrix as the transpose of the first one:\n", tr);
	const nc::Matrix4x4f inv = m1_.inverse();
	printMatrix("Creating a new matrix as the inverse of the first one:\n", inv);

	printf("The transpose and the inverse are equal: %d\n", tr == inv);

	ASSERT_TRUE(tr == inv);
	assertVectorsAreEqual(tr[0], inv[0]);
	assertVectorsAreEqual(tr[1], inv[1]);
	assertVectorsAreEqual(tr[2], inv[2]);
	assertVectorsAreEqual(tr[3], inv[3]);
}

TEST_F(Matrix4x4OperationsTest, TranslateIdentityInPlace)
{
	const nc::Vector3f tr(1.0f, 0.5, 0.75f);
	nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity;
	printMatrix("The identity matrix:\n", newMatrix);
	newMatrix.translate(tr);
	printf("Translating the identity matrix in place by <%.2f, %.2f, %.2f>:\n", tr.x, tr.y, tr.z);
	printMatrix(newMatrix);

	assertVectorsAreEqual(newMatrix[0], nc::Matrix4x4f::Identity[0]);
	assertVectorsAreEqual(newMatrix[1], nc::Matrix4x4f::Identity[1]);
	assertVectorsAreEqual(newMatrix[2], nc::Matrix4x4f::Identity[2]);
	assertVectorsAreEqual(newMatrix[3], tr.x, tr.y, tr.z, 1.0f);
}

TEST_F(Matrix4x4OperationsTest, RotateXIdentityInPlace)
{
	const float deg = 90.0f;
	nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity;
	printMatrix("The identity matrix:\n", newMatrix);
	newMatrix.rotateX(deg);
	printf("Rotating the identity matrix in place by %.2f degrees around X:\n", deg);
	printMatrix(newMatrix);

	assertVectorsAreNear(newMatrix[0], nc::Matrix4x4f::Identity[0], 0.0001f);
	assertVectorsAreNear(newMatrix[1], nc::Matrix4x4f::Identity[2], 0.0001f);
	assertVectorsAreNear(newMatrix[2], -nc::Matrix4x4f::Identity[1], 0.0001f);
	assertVectorsAreNear(newMatrix[3], nc::Matrix4x4f::Identity[3], 0.0001f);
}

TEST_F(Matrix4x4OperationsTest, RotateYIdentityInPlace)
{
	const float deg = 90.0f;
	nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity;
	printMatrix("The identity matrix:\n", newMatrix);
	newMatrix.rotateY(deg);
	printf("Rotating the identity matrix in place by %.2f degrees around Y:\n", deg);
	printMatrix(newMatrix);

	assertVectorsAreNear(newMatrix[0], -nc::Matrix4x4f::Identity[2], 0.0001f);
	assertVectorsAreNear(newMatrix[1], nc::Matrix4x4f::Identity[1], 0.0001f);
	assertVectorsAreNear(newMatrix[2], nc::Matrix4x4f::Identity[0], 0.0001f);
	assertVectorsAreNear(newMatrix[3], nc::Matrix4x4f::Identity[3], 0.0001f);
}

TEST_F(Matrix4x4OperationsTest, RotateZIdentityInPlace)
{
	const float deg = 90.0f;
	nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity;
	printMatrix("The identity matrix:\n", newMatrix);
	newMatrix.rotateZ(deg);
	printf("Rotating the identity matrix in place by %.2f degrees around Z:\n", deg);
	printMatrix(newMatrix);

	assertVectorsAreNear(newMatrix[0], nc::Matrix4x4f::Identity[1], 0.0001f);
	assertVectorsAreNear(newMatrix[1], -nc::Matrix4x4f::Identity[0], 0.0001f);
	assertVectorsAreNear(newMatrix[2], nc::Matrix4x4f::Identity[2], 0.0001f);
	assertVectorsAreNear(newMatrix[3], nc::Matrix4x4f::Identity[3], 0.0001f);
}

TEST_F(Matrix4x4OperationsTest, UniformScaleIdentityInPlace)
{
	const float sc = 0.5f;
	nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity;
	printMatrix("The identity matrix:\n", newMatrix);
	newMatrix.scale(sc);
	printf("Scaling the identity matrix in place by the scalar %.2f:\n", sc);
	printMatrix(newMatrix);

	assertVectorsAreEqual(newMatrix[0], nc::Matrix4x4f::Identity[0] * sc);
	assertVectorsAreEqual(newMatrix[1], nc::Matrix4x4f::Identity[1] * sc);
	assertVectorsAreEqual(newMatrix[2], nc::Matrix4x4f::Identity[2] * sc);
	assertVectorsAreEqual(newMatrix[3], nc::Matrix4x4f::Identity[3]);
}

TEST_F(Matrix4x4OperationsTest, ScaleIdentityInPlace)
{
	const nc::Vector3f sc(2.0f, 0.5, 0.75f);
	nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity;
	printMatrix("The identity matrix:\n", newMatrix);
	newMatrix.scale(sc);
	printf("Scaling the identity matrix in place by <%.2f, %.2f, %.2f>:\n", sc.x, sc.y, sc.z);
	printMatrix(newMatrix);

	assertVectorsAreEqual(newMatrix[0], nc::Matrix4x4f::Identity[0] * sc.x);
	assertVectorsAreEqual(newMatrix[1], nc::Matrix4x4f::Identity[1] * sc.y);
	assertVectorsAreEqual(newMatrix[2], nc::Matrix4x4f::Identity[2] * sc.z);
	assertVectorsAreEqual(newMatrix[3], nc::Matrix4x4f::Identity[3]);
}

TEST_F(Matrix4x4OperationsTest, TranslateIdentity)
{
	const nc::Vector3f tr(1.0f, 0.5, 0.75f);
	printMatrix("The identity matrix:\n", nc::Matrix4x4f::Identity);
	const nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity.translation(tr);
	printf("Translating the identity matrix by <%.2f, %.2f, %.2f>:\n", tr.x, tr.y, tr.z);
	printMatrix(newMatrix);

	assertVectorsAreEqual(newMatrix[0], nc::Matrix4x4f::Identity[0]);
	assertVectorsAreEqual(newMatrix[1], nc::Matrix4x4f::Identity[1]);
	assertVectorsAreEqual(newMatrix[2], nc::Matrix4x4f::Identity[2]);
	assertVectorsAreEqual(newMatrix[3], tr.x, tr.y, tr.z, 1.0f);
}

TEST_F(Matrix4x4OperationsTest, RotateXIdentity)
{
	const float deg = 90.0f;
	printMatrix("The identity matrix:\n", nc::Matrix4x4f::Identity);
	const nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity.rotationX(deg);
	printf("Rotating the identity matrix by %.2f degrees around X:\n", deg);
	printMatrix(newMatrix);

	assertVectorsAreNear(newMatrix[0], nc::Matrix4x4f::Identity[0], 0.0001f);
	assertVectorsAreNear(newMatrix[1], nc::Matrix4x4f::Identity[2], 0.0001f);
	assertVectorsAreNear(newMatrix[2], -nc::Matrix4x4f::Identity[1], 0.0001f);
	assertVectorsAreNear(newMatrix[3], nc::Matrix4x4f::Identity[3], 0.0001f);
}

TEST_F(Matrix4x4OperationsTest, RotateYIdentity)
{
	const float deg = 90.0f;
	printMatrix("The identity matrix:\n", nc::Matrix4x4f::Identity);
	const nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity.rotationY(deg);
	printf("Rotating the identity matrix by %.2f degrees around Y:\n", deg);
	printMatrix(newMatrix);

	assertVectorsAreNear(newMatrix[0], -nc::Matrix4x4f::Identity[2], 0.0001f);
	assertVectorsAreNear(newMatrix[1], nc::Matrix4x4f::Identity[1], 0.0001f);
	assertVectorsAreNear(newMatrix[2], nc::Matrix4x4f::Identity[0], 0.0001f);
	assertVectorsAreNear(newMatrix[3], nc::Matrix4x4f::Identity[3], 0.0001f);
}

TEST_F(Matrix4x4OperationsTest, RotateZIdentity)
{
	const float deg = 90.0f;
	printMatrix("The identity matrix:\n", nc::Matrix4x4f::Identity);
	const nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity.rotationZ(deg);
	printf("Rotating the identity matrix by %.2f degrees around Z:\n", deg);
	printMatrix(newMatrix);

	assertVectorsAreNear(newMatrix[0], nc::Matrix4x4f::Identity[1], 0.0001f);
	assertVectorsAreNear(newMatrix[1], -nc::Matrix4x4f::Identity[0], 0.0001f);
	assertVectorsAreNear(newMatrix[2], nc::Matrix4x4f::Identity[2], 0.0001f);
	assertVectorsAreNear(newMatrix[3], nc::Matrix4x4f::Identity[3], 0.0001f);
}

TEST_F(Matrix4x4OperationsTest, UniformScalingIdentity)
{
	const float sc = 0.5f;
	printMatrix("The identity matrix:\n", nc::Matrix4x4f::Identity);
	const nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity.scaling(sc);
	printf("Scaling the identity matrix by the scalar %.2f:\n", sc);
	printMatrix(newMatrix);

	assertVectorsAreEqual(newMatrix[0], nc::Matrix4x4f::Identity[0] * sc);
	assertVectorsAreEqual(newMatrix[1], nc::Matrix4x4f::Identity[1] * sc);
	assertVectorsAreEqual(newMatrix[2], nc::Matrix4x4f::Identity[2] * sc);
	assertVectorsAreEqual(newMatrix[3], nc::Matrix4x4f::Identity[3]);
}

TEST_F(Matrix4x4OperationsTest, ScalingIdentity)
{
	const nc::Vector3f sc(2.0f, 0.5, 0.75f);
	printMatrix("The identity matrix:\n", nc::Matrix4x4f::Identity);
	const nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity.scaling(sc);
	printf("Scaling the identity matrix by <%.2f, %.2f, %.2f>:\n", sc.x, sc.y, sc.z);
	printMatrix(newMatrix);

	assertVectorsAreEqual(newMatrix[0], nc::Matrix4x4f::Identity[0] * sc.x);
	assertVectorsAreEqual(newMatrix[1], nc::Matrix4x4f::Identity[1] * sc.y);
	assertVectorsAreEqual(newMatrix[2], nc::Matrix4x4f::Identity[2] * sc.z);
	assertVectorsAreEqual(newMatrix[3], nc::Matrix4x4f::Identity[3]);
}

TEST_F(Matrix4x4OperationsTest, RotateXIdentityInPlaceAndNot)
{
	const float deg = 45.0f;
	m1_ = nc::Matrix4x4f::Identity;
	m1_ *= nc::Matrix4x4f::rotationX(deg);
	printf("Rotating the identity matrix by %.2f degrees around X:\n", deg);
	printMatrix(m1_);
	nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity;
	newMatrix.rotateX(deg);
	printf("Rotating the identity matrix in place by %.2f degrees around X:\n", deg);
	printMatrix(newMatrix);

	assertVectorsAreNear(m1_[0], newMatrix[0], 0.0001f);
	assertVectorsAreNear(m1_[1], newMatrix[1], 0.0001f);
	assertVectorsAreNear(m1_[2], newMatrix[2], 0.0001f);
	assertVectorsAreNear(m1_[3], newMatrix[3], 0.0001f);
}

TEST_F(Matrix4x4OperationsTest, RotateYIdentityInPlaceAndNot)
{
	const float deg = 45.0f;
	m1_ = nc::Matrix4x4f::Identity;
	m1_ *= nc::Matrix4x4f::rotationY(deg);
	printf("Rotating the identity matrix by %.2f degrees around Y:\n", deg);
	printMatrix(m1_);
	nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity;
	newMatrix.rotateY(deg);
	printf("Rotating the identity matrix in place by %.2f degrees around Y:\n", deg);
	printMatrix(newMatrix);

	assertVectorsAreNear(m1_[0], newMatrix[0], 0.0001f);
	assertVectorsAreNear(m1_[1], newMatrix[1], 0.0001f);
	assertVectorsAreNear(m1_[2], newMatrix[2], 0.0001f);
	assertVectorsAreNear(m1_[3], newMatrix[3], 0.0001f);
}

TEST_F(Matrix4x4OperationsTest, RotateZIdentityInPlaceAndNot)
{
	const float deg = 45.0f;
	m1_ = nc::Matrix4x4f::Identity;
	m1_ *= nc::Matrix4x4f::rotationZ(deg);
	printf("Rotating the identity matrix by %.2f degrees around Z:\n", deg);
	printMatrix(m1_);
	nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity;
	newMatrix.rotateZ(deg);
	printf("Rotating the identity matrix in place by %.2f degrees around Z:\n", deg);
	printMatrix(newMatrix);

	assertVectorsAreNear(m1_[0], newMatrix[0], 0.0001f);
	assertVectorsAreNear(m1_[1], newMatrix[1], 0.0001f);
	assertVectorsAreNear(m1_[2], newMatrix[2], 0.0001f);
	assertVectorsAreNear(m1_[3], newMatrix[3], 0.0001f);
}

TEST_F(Matrix4x4OperationsTest, MultipleTransformationsInPlaceAndNot)
{
	const float x = 10.0f;
	const float y = 15.0f;
	const float z = 5.0f;
	const float degX = 45.0f;
	const float degY = 33.0f;
	const float degZ = 15.0f;
	const float sX = 2.0f;
	const float sY = 1.5f;
	const float sZ = -1.0f;
	const float x2 = -5.0f;
	const float y2 = 10.0f;
	const float z2 = 1.0f;

	m1_ = nc::Matrix4x4f::Identity;
	m1_ *= nc::Matrix4x4f::translation(x, y, z);
	m1_ *= nc::Matrix4x4f::rotationX(degX);
	m1_ *= nc::Matrix4x4f::rotationY(degY);
	m1_ *= nc::Matrix4x4f::rotationZ(degZ);
	m1_ *= nc::Matrix4x4f::scaling(sX, sY, sZ);
	m1_ *= nc::Matrix4x4f::translation(x2, y2, z2);
	printMatrix("Applying multiple transformations to the identity matrix:\n", m1_);

	nc::Matrix4x4f newMatrix = nc::Matrix4x4f::Identity;
	newMatrix.translate(x, y, z);
	newMatrix.rotateX(degX);
	newMatrix.rotateY(degY);
	newMatrix.rotateZ(degZ);
	newMatrix.scale(sX, sY, sZ);
	newMatrix.translate(x2, y2, z2);
	printMatrix("Applying multiple transformations in place to the identity matrix:\n", newMatrix);

	assertVectorsAreNear(m1_[0], newMatrix[0], 0.0001f);
	assertVectorsAreNear(m1_[1], newMatrix[1], 0.0001f);
	assertVectorsAreNear(m1_[2], newMatrix[2], 0.0001f);
	assertVectorsAreNear(m1_[3], newMatrix[3], 0.0001f);
}

TEST_F(Matrix4x4OperationsTest, Ortho)
{
	const nc::Matrix4x4f orthoMat = nc::Matrix4x4f::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f);
	printMatrix("Build an orthographic matrix to be equal to identity:\n", orthoMat);

	assertVectorsAreEqual(orthoMat[0], nc::Matrix4x4f::Identity[0]);
	assertVectorsAreEqual(orthoMat[1], nc::Matrix4x4f::Identity[1]);
	assertVectorsAreEqual(orthoMat[2], nc::Matrix4x4f::Identity[2]);
	assertVectorsAreEqual(orthoMat[3], nc::Matrix4x4f::Identity[3]);
}

TEST_F(Matrix4x4OperationsTest, Frustum)
{
	const nc::Matrix4x4f frustumMat = nc::Matrix4x4f::frustum(1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
	printMatrix("Build a simple frustum matrix:\n", frustumMat);

	assertVectorsAreEqual(frustumMat[0], nc::Matrix4x4f::Identity[0]);
	assertVectorsAreEqual(frustumMat[1], nc::Matrix4x4f::Identity[1]);
	assertVectorsAreEqual(frustumMat[2], -nc::Matrix4x4f::Identity[3]);
	assertVectorsAreEqual(frustumMat[3], nc::Matrix4x4f::Identity[2]);
}

TEST_F(Matrix4x4OperationsTest, Perspective)
{
	const nc::Matrix4x4f perspMat = nc::Matrix4x4f::perspective(90.0f, 1.0f, -1.0f, 1.0f);
	printMatrix("Build a simple perspective matrix:\n", perspMat);

	assertVectorsAreEqual(perspMat[0], nc::Matrix4x4f::Identity[0]);
	assertVectorsAreEqual(perspMat[1], nc::Matrix4x4f::Identity[1]);
	assertVectorsAreEqual(perspMat[2], -nc::Matrix4x4f::Identity[3]);
	assertVectorsAreEqual(perspMat[3], nc::Matrix4x4f::Identity[2]);
}

}
