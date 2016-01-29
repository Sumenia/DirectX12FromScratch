#pragma once

#include <cmath>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

#define DEG2RAD(x) ((x * M_PI) / 180.0)

namespace MiniEngine
{
	template<class T>
	class Matrix3
	{
	public:
		/// Data stored in column major order
		T data[9];

		//--------------------------[ constructors ]-------------------------------
		/**
		* Creates identity matrix
		*/
		Matrix3()
		{
			for (int i = 0; i < 9; i++)
				data[i] = (i % 4) ? 0 : 1;
		}

		/**
		* Copy matrix values from array (these data must be in column
		* major order!)
		*/
		Matrix3(const T * dt)
		{
			std::memcpy(data, dt, sizeof(T) * 9);
		}

		/**
		* Copy constructor.
		* @param src Data source for new created instance of Matrix3
		*/
		Matrix3(const Matrix3<T>& src)
		{
			std::memcpy(data, src.data, sizeof(T) * 9);
		}

		/**
		* Copy casting constructor.
		* @param src Data source for new created instance of Matrix3
		*/
		template<class FromT>
		Matrix3(const Matrix3<FromT>& src)
		{
			for (int i = 0; i < 9; i++)
			{
				data[i] = static_cast<T>(src.data[i]);
			}
		}

		/**
		* Resets matrix to be identity matrix
		*/
		void identity()
		{
			for (int i = 0; i < 9; i++)
				data[i] = (i % 4) ? 0 : 1;
		}

		/**
		* Creates rotation matrix by rotation around axis.
		* @param xDeg Angle (in degrees) of rotation around axis X.
		* @param yDeg Angle (in degrees) of rotation around axis Y.
		* @param zDeg Angle (in degrees) of rotation around axis Z.
		*/
		static Matrix3<T> createRotationAroundAxis(T xDeg, T yDeg, T zDeg)
		{
			T xRads(DEG2RAD(xDeg));
			T yRads(DEG2RAD(yDeg));
			T zRads(DEG2RAD(zDeg));

			Matrix3<T> ma, mb, mc;
			float ac = cos(xRads);
			float as = sin(xRads);
			float bc = cos(yRads);
			float bs = sin(yRads);
			float cc = cos(zRads);
			float cs = sin(zRads);

			ma.at(1, 1) = ac;
			ma.at(2, 1) = as;
			ma.at(1, 2) = -as;
			ma.at(2, 2) = ac;

			mb.at(0, 0) = bc;
			mb.at(2, 0) = -bs;
			mb.at(0, 2) = bs;
			mb.at(2, 2) = bc;

			mc.at(0, 0) = cc;
			mc.at(1, 0) = cs;
			mc.at(0, 1) = -cs;
			mc.at(1, 1) = cc;

			Matrix3<T> ret = ma * mb * mc;
			return ret;
		}

		/**
		* Creates rotation matrix from ODE Matrix.
		*/
		template<class It>
		static Matrix3<T> fromOde(const It* mat)
		{
			Matrix3<T> ret;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					ret.at(i, j) = static_cast<T>(mat[j * 4 + i]);
				}
			}
			return ret;
		}

		/**
		* Creates new matrix 3x3 from array that represents such matrix 3x3
		* as array of tightly packed elements in row major order.
		* @param arr An array of elements for 3x3 matrix in row major order.
		* @return An instance of Matrix3<T> representing @a arr
		*/
		template<class FromT>
		static Matrix3<T> fromRowMajorArray(const FromT* arr)
		{
			const T retData[] =
			{ static_cast<T>(arr[0]), static_cast<T>(arr[3]), static_cast<T>(arr[6]), static_cast<T>(arr[1]),
				static_cast<T>(arr[4]), static_cast<T>(arr[7]), static_cast<T>(arr[2]), static_cast<T>(arr[5]),
				static_cast<T>(arr[8]) };

			return retData;
		}

		/**
		* Creates new matrix 3x3 from array that represents such matrix 3x3
		* as array of tightly packed elements in column major order.
		* @param arr An array of elements for 3x3 matrix in column major order.
		* @return An instance of Matrix3<T> representing @a arr
		*/
		template<class FromT>
		static Matrix3<T> fromColumnMajorArray(const FromT* arr)
		{
			const T retData[] =
			{ static_cast<T>(arr[0]), static_cast<T>(arr[1]), static_cast<T>(arr[2]), static_cast<T>(arr[3]),
				static_cast<T>(arr[4]), static_cast<T>(arr[5]), static_cast<T>(arr[6]), static_cast<T>(arr[7]),
				static_cast<T>(arr[8]) };

			return retData;
		}

		//---------------------[ equiality operators ]------------------------------
		/**
		* Equality test operator
		* @param rhs Right hand side argument of binary operator.
		* @note Test of equality is based of threshold EPSILON value. To be two
		* values equal, must satisfy this condition all elements of matrix
		* | lhs[i] - rhs[i] | < EPSILON,
		* same for y-coordinate, z-coordinate, and w-coordinate.
		*/
		bool operator==(const Matrix3<T>& rhs) const
		{
			for (int i = 0; i < 9; i++)
			{
				if (std::fabs(data[i] - rhs.data[i]) >= EPSILON)
					return false;
			}
			return true;
		}

		/**
		* Inequality test operator
		* @param rhs Right hand side argument of binary operator.
		* @return not (lhs == rhs) :-P
		*/
		bool operator!=(const Matrix3<T>& rhs) const
		{
			return !(*this == rhs);
		}

		//---------------------[ access operators ]---------------------------------
		/**
		* Get reference to element at position (x,y).
		* @param x Number of column (0..2)
		* @param y Number of row (0..2)
		*/
		T& at(int x, int y)
		{
			assert(x >= 0 && x < 3);
			assert(y >= 0 && y < 3);
			return data[x * 3 + y];
		}

		/**
		* Get constant reference to element at position (x,y).
		* @param x Number of column (0..2)
		* @param y Number of row (0..2)
		*/
		const T& at(int x, int y) const
		{
			assert(x >= 0 && x < 3);
			assert(y >= 0 && y < 3);
			return data[x * 3 + y];
		}

		/**
		* Get reference to element at position (i,j), with math matrix notation
		* @param i Number of row (1..3)
		* @param j Number of column (1..3)
		*/
		T& operator()(int i, int j)
		{
			assert(i >= 1 && i <= 3);
			assert(j >= 1 && j <= 3);
			return data[(j - 1) * 3 + i - 1];
		}

		/**
		* Get constant reference to element at position (i,j), with math matrix notation
		* @param i Number of row (1..3)
		* @param j Number of column (1..3)
		*/
		const T& operator()(int i, int j) const
		{
			assert(i >= 1 && i <= 3);
			assert(j >= 1 && j <= 3);
			return data[(j - 1) * 3 + i - 1];
		}

		/**
		* Copy operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix3<T>& operator=(const Matrix3<T>& rhs)
		{
			std::memcpy(data, rhs.data, sizeof(T) * 9);
			return *this;
		}

		/**
		* Copy casting operator
		* @param rhs Right hand side argument of binary operator.
		*/
		template<class FromT>
		Matrix3<T>& operator=(const Matrix3<FromT>& rhs)
		{
			for (int i = 0; i < 9; i++)
			{
				data[i] = static_cast<T>(rhs.data[i]);
			}
			return *this;
		}

		/**
		* Copy operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix3<T>& operator=(const T* rhs)
		{
			std::memcpy(data, rhs, sizeof(T) * 9);
			return *this;
		}

		/*Matrix3<T> & operator=(const double* m)
		{
		for (int i = 0; i < 9; i++) data[i] = (T)m[i];
		return * this;
		}*/

		//--------------------[ matrix with matrix operations ]---------------------
		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix3<T> operator+(const Matrix3<T>& rhs) const
		{
			Matrix3<T> ret;
			for (int i = 0; i < 9; i++)
				ret.data[i] = data[i] + rhs.data[i];
			return ret;
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix3<T> operator-(const Matrix3<T>& rhs) const
		{
			Matrix3<T> ret;
			for (int i = 0; i < 9; i++)
				ret.data[i] = data[i] - rhs.data[i];
			return ret;
		}

		//--------------------[ matrix with scalar operations ]---------------------
		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix3<T> operator+(T rhs) const
		{
			Matrix3<T> ret;
			for (int i = 0; i < 9; i++)
				ret.data[i] = data[i] + rhs;
			return ret;
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix3<T> operator-(T rhs) const
		{
			Matrix3<T> ret;
			for (int i = 0; i < 9; i++)
				ret.data[i] = data[i] - rhs;
			return ret;
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix3<T> operator*(T rhs) const
		{
			Matrix3<T> ret;
			for (int i = 0; i < 9; i++)
				ret.data[i] = data[i] * rhs;
			return ret;
		}

		/**
		* Division operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix3<T> operator/(T rhs) const
		{
			Matrix3<T> ret;
			for (int i = 0; i < 9; i++)
				ret.data[i] = data[i] / rhs;
			return ret;
		}

		//--------------------[ multiply operators ]--------------------------------
		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T> operator*(const Vector3<T>& rhs) const
		{
			return Vector3<T>(data[0] * rhs.x + data[3] * rhs.y + data[6] * rhs.z,
				data[1] * rhs.x + data[4] * rhs.y + data[7] * rhs.z,
				data[2] * rhs.x + data[5] * rhs.y + data[8] * rhs.z);
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix3<T> operator*(Matrix3<T> rhs) const
		{
			static Matrix3<T> w;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					T n = 0;
					for (int k = 0; k < 3; k++)
						n += rhs.at(i, k) * at(k, j);
					w.at(i, j) = n;
				}
			}
			return w;

		}

		//---------------------------[ misc operations ]----------------------------
		/**
		* Transpose matrix.
		*/
		Matrix3<T> transpose()
		{
			Matrix3<T> ret;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					ret.at(i, j) = at(j, i);
				}
			}
			return ret;
		}

		/**
		* Linear interpolation of two matrices
		* @param fact Factor of interpolation. For translation from positon
		* of this matrix (lhs) to matrix rhs, values of factor goes from 0.0 to 1.0.
		* @param rhs Second Matrix for interpolation
		* @note However values of fact parameter are reasonable only in interval
		* [0.0 , 1.0], you can pass also values outside of this interval and you
		* can get result (extrapolation?)
		*/
		Matrix3<T> lerp(T fact, const Matrix3<T>& rhs) const
		{
			Matrix3<T> ret = (*this) + (rhs - (*this)) * fact;
			return ret;
		}

		T det()
		{
			return +at(0, 0) * at(1, 1) * at(2, 2) + at(0, 1) * at(1, 2) * at(2, 0) + at(0, 2) * at(1, 0) * at(2, 1)
				- at(0, 0) * at(1, 2) * at(2, 1) - at(0, 1) * at(1, 0) * at(2, 2) - at(0, 2) * at(1, 1) * at(2, 0);
		}

		/**
		* Computes inverse matrix
		* @return Inverse matrix of this matrix.
		*/
		Matrix3<T> inverse()
		{
			Matrix3<T> ret;
			ret.at(0, 0) = at(1, 1) * at(2, 2) - at(2, 1) * at(1, 2);
			ret.at(0, 1) = at(2, 1) * at(0, 2) - at(0, 1) * at(2, 2);
			ret.at(0, 2) = at(0, 1) * at(1, 2) - at(1, 1) * at(0, 2);
			ret.at(1, 0) = at(2, 0) * at(1, 2) - at(1, 0) * at(2, 2);
			ret.at(1, 1) = at(0, 0) * at(2, 2) - at(2, 0) * at(0, 2);
			ret.at(1, 2) = at(1, 0) * at(0, 2) - at(0, 0) * at(1, 2);
			ret.at(2, 0) = at(1, 0) * at(2, 1) - at(2, 0) * at(1, 1);
			ret.at(2, 1) = at(2, 0) * at(0, 1) - at(0, 0) * at(2, 1);
			ret.at(2, 2) = at(0, 0) * at(1, 1) - at(1, 0) * at(0, 1);
			return ret * (1.0f / det());
		}

		//-------------[ conversion ]-----------------------------

		/**
		* Conversion to pointer operator
		* @return Pointer to internally stored (in management of class Matrix3<T>)
		* used for passing Matrix3<T> values to gl*[fd]v functions.
		*/
		operator T*()
		{
			return (T*)data;
		}

		/**
		* Conversion to pointer operator
		* @return Constant Pointer to internally stored (in management of class Matrix3<T>)
		* used for passing Matrix3<T> values to gl*[fd]v functions.
		*/
		operator const T*() const
		{
			return (const T*)data;
		}

		//----------[ output operator ]----------------------------
		/**
		* Output to stream operator
		* @param lhs Left hand side argument of operator (commonly ostream instance).
		* @param rhs Right hand side argument of operator.
		* @return Left hand side argument - the ostream object passed to operator.
		*/
		friend std::ostream& operator <<(std::ostream& lhs, const Matrix3<T>& rhs)
		{
			for (int i = 0; i < 3; i++)
			{
				lhs << "|\t";
				for (int j = 0; j < 3; j++)
				{
					lhs << rhs.at(j, i) << "\t";
				}
				lhs << "|" << std::endl;
			}
			return lhs;
		}

		/**
		* Gets string representation.
		*/
		std::string toString() const
		{
			std::ostringstream oss;
			oss << *this;
			return oss.str();
		}
	};

	/// Matrix 3x3 of floats
	typedef Matrix3<float> Matrix3f;
	/// Matrix 3x3 of doubles
	typedef Matrix3<double> Matrix3d;
	/// Matrix 3x3 of int
	typedef Matrix3<int> Matrix3i;

	/**
	* Class for matrix 4x4
	* @note Data stored in this matrix are in column major order. This arrangement suits OpenGL.
	* If you're using row major matrix, consider using fromRowMajorArray as way for construction
	* Matrix4<T> instance.
	*/
	template<class T>
	class Matrix4
	{
	public:
		/// Data stored in column major order
		T data[16];

		//--------------------------[ constructors ]-------------------------------
		/**
		*Creates identity matrix
		*/
		Matrix4()
		{
			for (int i = 0; i < 16; i++)
				data[i] = (i % 5) ? 0 : 1;
		}

		/**
		* Copy matrix values from array (these data must be in column
		* major order!)
		*/
		Matrix4(const T * dt)
		{
			std::memcpy(data, dt, sizeof(T) * 16);
		}

		/**
		* Copy constructor.
		* @param src Data source for new created instance of Matrix4.
		*/
		Matrix4(const Matrix4<T>& src)
		{
			std::memcpy(data, src.data, sizeof(T) * 16);
		}

		/**
		* Copy casting constructor.
		* @param src Data source for new created instance of Matrix4.
		*/
		template<class FromT>
		Matrix4(const Matrix4<FromT>& src)
		{
			for (int i = 0; i < 16; i++)
			{
				data[i] = static_cast<T>(src.data[i]);
			}
		}

		/**
		* Resets matrix to be identity matrix
		*/
		void identity()
		{
			for (int i = 0; i < 16; i++)
				data[i] = (i % 5) ? 0 : 1;
		}

		/**
		* Creates rotation matrix by rotation around axis.
		* @param xDeg Angle (in degrees) of rotation around axis X.
		* @param yDeg Angle (in degrees) of rotation around axis Y.
		* @param zDeg Angle (in degrees) of rotation around axis Z.
		*/
		static Matrix4<T> createRotationAroundAxis(T xDeg, T yDeg, T zDeg)
		{
			T xRads(DEG2RAD(xDeg));
			T yRads(DEG2RAD(yDeg));
			T zRads(DEG2RAD(zDeg));

			Matrix4<T> ma, mb, mc;
			float ac = cos(xRads);
			float as = sin(xRads);
			float bc = cos(yRads);
			float bs = sin(yRads);
			float cc = cos(zRads);
			float cs = sin(zRads);

			ma.at(1, 1) = ac;
			ma.at(2, 1) = as;
			ma.at(1, 2) = -as;
			ma.at(2, 2) = ac;

			mb.at(0, 0) = bc;
			mb.at(2, 0) = -bs;
			mb.at(0, 2) = bs;
			mb.at(2, 2) = bc;

			mc.at(0, 0) = cc;
			mc.at(1, 0) = cs;
			mc.at(0, 1) = -cs;
			mc.at(1, 1) = cc;

			/*std::cout << "RotVec = " << a << "," << b << "," << c << std::endl;
			std::cout << "Rx = " << std::endl << ma;
			std::cout << "Ry = " << std::endl << mb;
			std::cout << "Rz = " << std::endl << mc;*/

			Matrix4<T> ret = ma * mb * mc;
			//std::cout << "Result = " << std::endl << ma * (mb * mc);

			return ret;
		}

		/// Creates translation matrix
		/**
		* Creates translation matrix.
		* @param x X-direction translation
		* @param y Y-direction translation
		* @param z Z-direction translation
		* @param w for W-coordinate translation (implicitly set to 1)
		*/
		static Matrix4<T> createTranslation(T x, T y, T z, T w = 1)
		{
			Matrix4 ret;
			ret.at(3, 0) = x;
			ret.at(3, 1) = y;
			ret.at(3, 2) = z;
			ret.at(3, 3) = w;

			return ret;
		}

		/**
		* Create scale matrix with @a sx, @a sy, and @a sz
		* being values of matrix main diagonal.
		* @param sx Scale in X-axis
		* @param sy Scale in Y-axis
		* @param sz Scale in Z-axis
		* @return Transform matrix 4x4 with scale transformation.
		*/
		static Matrix4<T> createScale(T sx, T sy, T sz)
		{
			Matrix4<T> ret;
			ret.at(0, 0) = sx;
			ret.at(1, 1) = sy;
			ret.at(2, 2) = sz;

			return ret;
		}

		/**
		* Creates new view matrix to look from specified position @a eyePos to specified position @a centerPos
		* @param eyePos A position of camera
		* @param centerPos A position where camera looks-at
		* @param upDir Direction of up vector
		* @return Resulting view matrix that looks from and at specific position.
		*/
		static Matrix4<T> createLookAt(const Vector3<T>& eyePos, const Vector3<T>& centerPos, const Vector3<T>& upDir)
		{
			Vector3<T> forward, side, up;
			Matrix4<T> m;

			forward = centerPos - eyePos;
			up = upDir;

			forward.normalize();

			// Side = forward x up
			side = forward.crossProduct(up);
			side.normalize();

			// Recompute up as: up = side x forward
			up = side.crossProduct(forward);

			m.at(0, 0) = side.x;
			m.at(1, 0) = side.y;
			m.at(2, 0) = side.z;

			m.at(0, 1) = up.x;
			m.at(1, 1) = up.y;
			m.at(2, 1) = up.z;

			m.at(0, 2) = -forward.x;
			m.at(1, 2) = -forward.y;
			m.at(2, 2) = -forward.z;

			m = m * Matrix4<T>::createTranslation(-eyePos.x, -eyePos.y, -eyePos.z);
			return m;
		}


		/**
		* Creates OpenGL compatible perspective projection according specified frustum parameters.
		*
		* @param left Specify the coordinate for the left vertical clipping plane,
		* @param right Specify the coordinate for the right vertical clipping plane.
		* @param bottom Specify the coordinate for the bottom horizontal clipping plane,
		* @param top Specify the coordinate for the top horizontal clipping plane.
		* @param zNear Specify the distance to the near clipping plane.  Distance must be positive.
		* @param zFar Specify the distance to the far depth clipping plane.  Distance must be positive.
		*
		* @return Projection matrix for specified frustum.
		*/
		static Matrix4<T> createFrustum(T left, T right, T bottom, T top, T zNear, T zFar)
		{
			/*
			*
			2 zNear
			------------       0              A              0
			right - left

			2 zNear
			0         ------------        B              0
			top - bottom

			0              0              C              D

			0              0              -1             0

			A = (right + left) / (right - left)

			B = (top + bottom) / (top - bottom)

			C = - (zFar + zNear) / (zFar - zNear)

			D = - (2 zFar zNear) / (zFar - zNear)
			*
			*/
			Matrix4<T> ret;

			const T invWidth = 1.0 / (right - left);
			const T invHeight = 1.0 / (top - bottom);
			const T invDepth = 1.0 / (zFar - zNear);

			const T twoZNear = 2 * zNear;

			ret.at(0, 0) = twoZNear * invWidth;
			ret.at(1, 1) = twoZNear * invHeight;

			ret.at(2, 0) = (right + left) * invWidth;
			ret.at(2, 1) = (top + bottom) * invHeight;
			ret.at(2, 2) = -(zFar + zNear) * invDepth;
			ret.at(2, 3) = -1;

			ret.at(3, 2) = -twoZNear * zFar * invDepth;

			return ret;
		}

		/**
		* Creates OpenGL compatible orthographic projection matrix.
		* @param left Specify the coordinate for the left vertical clipping plane,
		* @param right Specify the coordinate for the right vertical clipping plane.
		* @param bottom Specify the coordinate for the bottom horizontal clipping plane,
		* @param top Specify the coordinate for the top horizontal clipping plane.
		* @param zNear Specify the distance to the nearer depth clipping plane.
		*       This value is negative if the plane is to be behind the viewer,
		* @param zFar Specify the distance to the farther depth clipping plane.
		*       This value is negative if the plane is to be behind the viewer.
		* @return Othrographic projection matrix.
		*/
		static Matrix4<T> createOrtho(T left, T right, T bottom, T top, T zNear, T zFar)
		{
			/*
			2
			------------       0              0              tx
			right - left

			2
			0         ------------        0              ty
			top - bottom

			-2
			0              0         ------------        tz
			zFar-zNear

			0              0              0              1

			where

			tx = - (right + left) / (right - left)

			ty = - (top + bottom) / (top - bottom)

			tz = - (zFar + zNear) / (zFar - zNear)

			*/

			const T invWidth = 1.0 / (right - left);
			const T invHeight = 1.0 / (top - bottom);
			const T invDepth = 1.0 / (zFar - zNear);

			Matrix4<T> ret;

			ret.at(0, 0) = 2 * invWidth;
			ret.at(1, 1) = 2 * invHeight;
			ret.at(2, 2) = -2 * invDepth;

			ret.at(3, 0) = -(right + left) * invWidth;
			ret.at(3, 1) = -(top + bottom) * invHeight;
			ret.at(3, 2) = -(zFar + zNear) * invDepth;

			return ret;
		}

		/**
		* Creates new matrix 4x4 from array that represents such matrix 4x4
		* as array of tightly packed elements in row major order.
		* @param arr An array of elements for 4x4 matrix in row major order.
		* @return An instance of Matrix4<T> representing @a arr
		*/
		template<class FromT>
		static Matrix4<T> fromRowMajorArray(const FromT* arr)
		{
			const T retData[] =
			{ static_cast<T>(arr[0]), static_cast<T>(arr[4]), static_cast<T>(arr[8]), static_cast<T>(arr[12]),
				static_cast<T>(arr[1]), static_cast<T>(arr[5]), static_cast<T>(arr[9]), static_cast<T>(arr[13]),
				static_cast<T>(arr[2]), static_cast<T>(arr[6]), static_cast<T>(arr[10]), static_cast<T>(arr[14]),
				static_cast<T>(arr[3]), static_cast<T>(arr[7]), static_cast<T>(arr[11]), static_cast<T>(arr[15]) };

			return retData;
		}

		/**
		* Creates new matrix 4x4 from array that represents such matrix 4x4
		* as array of tightly packed elements in column major order.
		* @param arr An array of elements for 4x4 matrix in column major order.
		* @return An instance of Matrix4<T> representing @a arr
		*/
		template<class FromT>
		static Matrix4<T> fromColumnMajorArray(const FromT* arr)
		{
			const T retData[] =
			{ static_cast<T>(arr[0]), static_cast<T>(arr[1]), static_cast<T>(arr[2]), static_cast<T>(arr[3]),
				static_cast<T>(arr[4]), static_cast<T>(arr[5]), static_cast<T>(arr[6]), static_cast<T>(arr[7]),
				static_cast<T>(arr[8]), static_cast<T>(arr[9]), static_cast<T>(arr[10]), static_cast<T>(arr[11]),
				static_cast<T>(arr[12]), static_cast<T>(arr[13]), static_cast<T>(arr[14]), static_cast<T>(arr[15]) };

			return retData;
		}

		//---------------------[ Equality operators ]------------------------------
		/**
		* Equality test operator
		* @param rhs Right hand side argument of binary operator.
		* @note Test of equality is based of threshold EPSILON value. To be two
		* values equal, must satisfy this condition all elements of matrix
		* | lhs[i] - rhs[i] | < EPSILON,
		* same for y-coordinate, z-coordinate, and w-coordinate.
		*/
		bool operator==(const Matrix4<T>& rhs) const
		{
			for (int i = 0; i < 16; i++)
			{
				if (std::fabs(data[i] - rhs.data[i]) >= EPSILON
					)
					return false;
			}
			return true;
		}

		/**
		* Inequality test operator
		* @param rhs Right hand side argument of binary operator.
		* @return not (lhs == rhs) :-P
		*/
		bool operator!=(const Matrix4<T>& rhs) const
		{
			return !(*this == rhs);
		}

		//---------------------[ access operators ]---------------------------------
		/**
		* Get reference to element at postion (x,y).
		* @param x Number of column (0..3)
		* @param y Number of row (0..3)
		*/
		T& at(int x, int y)
		{
			assert(x >= 0 && x < 4);
			assert(y >= 0 && y < 4);
			return data[x * 4 + y];
		}

		/**
		* Get constant reference to element at position (x,y).
		* @param x Number of column (0..3)
		* @param y Number of row (0..3)
		*/
		const T& at(int x, int y) const
		{
			assert(x >= 0 && x < 4);
			assert(y >= 0 && y < 4);
			return data[x * 4 + y];
		}

		/**
		* Get reference to element at position (i,j), with math matrix notation
		* @param i Number of row (1..4)
		* @param j Number of column (1..4)
		*/
		T& operator()(int i, int j)
		{
			assert(i >= 1 && i <= 4);
			assert(j >= 1 && j <= 4);
			return data[(j - 1) * 4 + i - 1];
		}

		/**
		* Get constant reference to element at position (i,j), with math matrix notation
		* @param i Number of row (1..4)
		* @param j Number of column (1..4)
		*/
		const T& operator()(int i, int j) const
		{
			assert(i >= 1 && i <= 4);
			assert(j >= 1 && j <= 4);
			return data[(j - 1) * 4 + i - 1];
		}

		/**
		* Sets translation part of matrix.
		*
		* @param v Vector of translation to be set.
		*/
		void setTranslation(const Vector3<T>& v)
		{
			at(3, 0) = v.x;
			at(3, 1) = v.y;
			at(3, 2) = v.z;
			at(3, 3) = 1;
		}

		Vector3<T> getTranslation() const
		{
			return Vector3<T>(at(3, 0), at(3, 1), at(3, 2));
		}

		/**
		* Sets rotation part (matrix 3x3) of matrix.
		*
		* @param m Rotation part of matrix
		*/
		void setRotation(const Matrix3<T>& m)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					at(i, j) = m.at(i, j);
				}
			}
		}

		/**
		* Gets matrix scale
		* @return Scales (i.e. first three values from matrix diagonal.
		*/
		Vector3<T> getScale() const
		{
			return Vector3<T>(at(0, 0), at(1, 1), at(2, 2));
		}

		/**
		* Sets matrix uniform scale values
		* @param s Uniform scale value
		*/
		void setScale(T s)
		{
			at(0, 0) = at(1, 1) = at(2, 2) = s;
		}

		/**
		* Sets matrix scale for all axes.
		* @param sx X-axis scale factor
		* @param sy Y-axis scale factor
		* @param sz Z-axis scale factor
		*/
		void setScale(T sx, T sy, T sz)
		{
			at(0, 0) = sx;
			at(1, 1) = sy;
			at(2, 2) = sz;
		}

		/**
		* Sets matrix scale for all axes.
		* @param s Scale factors for X, Y, and Z coordinate.
		*/
		void setScale(const Vector3<T>& s)
		{
			at(0, 0) = s.x;
			at(1, 1) = s.y;
			at(2, 2) = s.z;
		}

		/**
		* Copy operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix4<T>& operator=(const Matrix4<T>& rhs)
		{
			std::memcpy(data, rhs.data, sizeof(T) * 16);
			return *this;
		}

		/**
		* Copy casting operator
		* @param rhs Right hand side argument of binary operator.
		*/
		template<class FromT>
		Matrix4<T>& operator=(const Matrix4<FromT>& rhs)
		{
			for (int i = 0; i < 16; i++)
			{
				data[i] = static_cast<T>(rhs.data[i]);
			}
			return *this;
		}

		/**
		* Copy operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix4<T>& operator=(const T* rhs)
		{
			std::memcpy(data, rhs, sizeof(T) * 16);
			return *this;
		}

		/*Matrix4<T> & operator=(const double* m)
		{
		for (int i = 0; i < 16; i++) data[i] = (T)m[i];
		return * this;
		}*/

		//--------------------[ matrix with matrix operations ]---------------------
		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix4<T> operator+(const Matrix4<T>& rhs) const
		{
			Matrix4<T> ret;
			for (int i = 0; i < 16; i++)
				ret.data[i] = data[i] + rhs.data[i];
			return ret;
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix4<T> operator-(const Matrix4<T>& rhs) const
		{
			Matrix4<T> ret;
			for (int i = 0; i < 16; i++)
				ret.data[i] = data[i] - rhs.data[i];
			return ret;
		}

		//--------------------[ matrix with scalar operations ]---------------------
		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix4<T> operator+(T rhs) const
		{
			Matrix4<T> ret;
			for (int i = 0; i < 16; i++)
				ret.data[i] = data[i] + rhs;
			return ret;
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix4<T> operator-(T rhs) const
		{
			Matrix4<T> ret;
			for (int i = 0; i < 16; i++)
				ret.data[i] = data[i] - rhs;
			return ret;
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix4<T> operator*(T rhs) const
		{
			Matrix4<T> ret;
			for (int i = 0; i < 16; i++)
				ret.data[i] = data[i] * rhs;
			return ret;
		}

		/**
		* Division operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix4<T> operator/(T rhs) const
		{
			Matrix4<T> ret;
			for (int i = 0; i < 16; i++)
				ret.data[i] = data[i] / rhs;
			return ret;
		}

		//--------------------[ multiply operators ]--------------------------------
		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T> operator*(const Vector4<T>& rhs) const
		{
			return Vector4<T>(data[0] * rhs.x + data[4] * rhs.y + data[8] * rhs.z + data[12] * rhs.w,
				data[1] * rhs.x + data[5] * rhs.y + data[9] * rhs.z + data[13] * rhs.w,
				data[2] * rhs.x + data[6] * rhs.y + data[10] * rhs.z + data[14] * rhs.w,
				data[3] * rhs.x + data[7] * rhs.y + data[11] * rhs.z + data[15] * rhs.w);

		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T> operator*(const Vector3<T>& rhs) const
		{
			return Vector3<T>(data[0] * rhs.x + data[4] * rhs.y + data[8] * rhs.z,
				data[1] * rhs.x + data[5] * rhs.y + data[9] * rhs.z,
				data[2] * rhs.x + data[6] * rhs.y + data[10] * rhs.z);
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Matrix4<T> operator*(Matrix4<T> rhs) const
		{
			static Matrix4<T> w;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					T n = 0;
					for (int k = 0; k < 4; k++)
						n += rhs.at(i, k) * at(k, j);
					w.at(i, j) = n;
				}
			}
			return w;

		}

		//---------------------------[ misc operations ]----------------------------

		/**
		* Computes determinant of matrix
		* @return Determinant of matrix
		* @note This function does 3 * 4 * 6 muls, 3 * 6 adds.
		*/
		T det()
		{

			return +at(3, 0) * at(2, 1) * at(1, 2) * at(0, 3) - at(2, 0) * at(3, 1) * at(1, 2) * at(0, 3)
				- at(3, 0) * at(1, 1) * at(2, 2) * at(0, 3) + at(1, 0) * at(3, 1) * at(2, 2) * at(0, 3)

				+ at(2, 0) * at(1, 1) * at(3, 2) * at(0, 3) - at(1, 0) * at(2, 1) * at(3, 2) * at(0, 3)
				- at(3, 0) * at(2, 1) * at(0, 2) * at(1, 3) + at(2, 0) * at(3, 1) * at(0, 2) * at(1, 3)

				+ at(3, 0) * at(0, 1) * at(2, 2) * at(1, 3) - at(0, 0) * at(3, 1) * at(2, 2) * at(1, 3)
				- at(2, 0) * at(0, 1) * at(3, 2) * at(1, 3) + at(0, 0) * at(2, 1) * at(3, 2) * at(1, 3)

				+ at(3, 0) * at(1, 1) * at(0, 2) * at(2, 3) - at(1, 0) * at(3, 1) * at(0, 2) * at(2, 3)
				- at(3, 0) * at(0, 1) * at(1, 2) * at(2, 3) + at(0, 0) * at(3, 1) * at(1, 2) * at(2, 3)

				+ at(1, 0) * at(0, 1) * at(3, 2) * at(2, 3) - at(0, 0) * at(1, 1) * at(3, 2) * at(2, 3)
				- at(2, 0) * at(1, 1) * at(0, 2) * at(3, 3) + at(1, 0) * at(2, 1) * at(0, 2) * at(3, 3)

				+ at(2, 0) * at(0, 1) * at(1, 2) * at(3, 3) - at(0, 0) * at(2, 1) * at(1, 2) * at(3, 3)
				- at(1, 0) * at(0, 1) * at(2, 2) * at(3, 3) + at(0, 0) * at(1, 1) * at(2, 2) * at(3, 3);

		}

		/**
		* Computes inverse matrix
		* @return Inverse matrix of this matrix.
		* @note This is a little bit time consuming operation
		* (16 * 6 * 3 muls, 16 * 5 adds + det() + mul() functions)
		*/
		Matrix4<T> inverse()
		{
			Matrix4<T> ret;

			ret.at(0, 0) = +at(2, 1) * at(3, 2) * at(1, 3) - at(3, 1) * at(2, 2) * at(1, 3) + at(3, 1) * at(1, 2) * at(2, 3)
				- at(1, 1) * at(3, 2) * at(2, 3) - at(2, 1) * at(1, 2) * at(3, 3) + at(1, 1) * at(2, 2) * at(3, 3);

			ret.at(1, 0) = +at(3, 0) * at(2, 2) * at(1, 3) - at(2, 0) * at(3, 2) * at(1, 3) - at(3, 0) * at(1, 2) * at(2, 3)
				+ at(1, 0) * at(3, 2) * at(2, 3) + at(2, 0) * at(1, 2) * at(3, 3) - at(1, 0) * at(2, 2) * at(3, 3);

			ret.at(2, 0) = +at(2, 0) * at(3, 1) * at(1, 3) - at(3, 0) * at(2, 1) * at(1, 3) + at(3, 0) * at(1, 1) * at(2, 3)
				- at(1, 0) * at(3, 1) * at(2, 3) - at(2, 0) * at(1, 1) * at(3, 3) + at(1, 0) * at(2, 1) * at(3, 3);

			ret.at(3, 0) = +at(3, 0) * at(2, 1) * at(1, 2) - at(2, 0) * at(3, 1) * at(1, 2) - at(3, 0) * at(1, 1) * at(2, 2)
				+ at(1, 0) * at(3, 1) * at(2, 2) + at(2, 0) * at(1, 1) * at(3, 2) - at(1, 0) * at(2, 1) * at(3, 2);

			ret.at(0, 1) = +at(3, 1) * at(2, 2) * at(0, 3) - at(2, 1) * at(3, 2) * at(0, 3) - at(3, 1) * at(0, 2) * at(2, 3)
				+ at(0, 1) * at(3, 2) * at(2, 3) + at(2, 1) * at(0, 2) * at(3, 3) - at(0, 1) * at(2, 2) * at(3, 3);

			ret.at(1, 1) = +at(2, 0) * at(3, 2) * at(0, 3) - at(3, 0) * at(2, 2) * at(0, 3) + at(3, 0) * at(0, 2) * at(2, 3)
				- at(0, 0) * at(3, 2) * at(2, 3) - at(2, 0) * at(0, 2) * at(3, 3) + at(0, 0) * at(2, 2) * at(3, 3);

			ret.at(2, 1) = +at(3, 0) * at(2, 1) * at(0, 3) - at(2, 0) * at(3, 1) * at(0, 3) - at(3, 0) * at(0, 1) * at(2, 3)
				+ at(0, 0) * at(3, 1) * at(2, 3) + at(2, 0) * at(0, 1) * at(3, 3) - at(0, 0) * at(2, 1) * at(3, 3);

			ret.at(3, 1) = +at(2, 0) * at(3, 1) * at(0, 2) - at(3, 0) * at(2, 1) * at(0, 2) + at(3, 0) * at(0, 1) * at(2, 2)
				- at(0, 0) * at(3, 1) * at(2, 2) - at(2, 0) * at(0, 1) * at(3, 2) + at(0, 0) * at(2, 1) * at(3, 2);

			ret.at(0, 2) = +at(1, 1) * at(3, 2) * at(0, 3) - at(3, 1) * at(1, 2) * at(0, 3) + at(3, 1) * at(0, 2) * at(1, 3)
				- at(0, 1) * at(3, 2) * at(1, 3) - at(1, 1) * at(0, 2) * at(3, 3) + at(0, 1) * at(1, 2) * at(3, 3);

			ret.at(1, 2) = +at(3, 0) * at(1, 2) * at(0, 3) - at(1, 0) * at(3, 2) * at(0, 3) - at(3, 0) * at(0, 2) * at(1, 3)
				+ at(0, 0) * at(3, 2) * at(1, 3) + at(1, 0) * at(0, 2) * at(3, 3) - at(0, 0) * at(1, 2) * at(3, 3);

			ret.at(2, 2) = +at(1, 0) * at(3, 1) * at(0, 3) - at(3, 0) * at(1, 1) * at(0, 3) + at(3, 0) * at(0, 1) * at(1, 3)
				- at(0, 0) * at(3, 1) * at(1, 3) - at(1, 0) * at(0, 1) * at(3, 3) + at(0, 0) * at(1, 1) * at(3, 3);

			ret.at(3, 2) = +at(3, 0) * at(1, 1) * at(0, 2) - at(1, 0) * at(3, 1) * at(0, 2) - at(3, 0) * at(0, 1) * at(1, 2)
				+ at(0, 0) * at(3, 1) * at(1, 2) + at(1, 0) * at(0, 1) * at(3, 2) - at(0, 0) * at(1, 1) * at(3, 2);

			ret.at(0, 3) = +at(2, 1) * at(1, 2) * at(0, 3) - at(1, 1) * at(2, 2) * at(0, 3) - at(2, 1) * at(0, 2) * at(1, 3)
				+ at(0, 1) * at(2, 2) * at(1, 3) + at(1, 1) * at(0, 2) * at(2, 3) - at(0, 1) * at(1, 2) * at(2, 3);

			ret.at(1, 3) = +at(1, 0) * at(2, 2) * at(0, 3) - at(2, 0) * at(1, 2) * at(0, 3) + at(2, 0) * at(0, 2) * at(1, 3)
				- at(0, 0) * at(2, 2) * at(1, 3) - at(1, 0) * at(0, 2) * at(2, 3) + at(0, 0) * at(1, 2) * at(2, 3);

			ret.at(2, 3) = +at(2, 0) * at(1, 1) * at(0, 3) - at(1, 0) * at(2, 1) * at(0, 3) - at(2, 0) * at(0, 1) * at(1, 3)
				+ at(0, 0) * at(2, 1) * at(1, 3) + at(1, 0) * at(0, 1) * at(2, 3) - at(0, 0) * at(1, 1) * at(2, 3);

			ret.at(3, 3) = +at(1, 0) * at(2, 1) * at(0, 2) - at(2, 0) * at(1, 1) * at(0, 2) + at(2, 0) * at(0, 1) * at(1, 2)
				- at(0, 0) * at(2, 1) * at(1, 2) - at(1, 0) * at(0, 1) * at(2, 2) + at(0, 0) * at(1, 1) * at(2, 2);

			return ret / det();
		}

		/**
		* Transpose matrix.
		*/
		Matrix4<T> transpose()
		{
			Matrix4<T> ret;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					ret.at(i, j) = at(j, i);
				}
			}
			return ret;
		}

		/**
		* Linear interpolation of two matrices
		* @param fact Factor of interpolation. For translation from positon
		* of this matrix (lhs) to matrix rhs, values of factor goes from 0.0 to 1.0.
		* @param rhs Second Matrix for interpolation
		* @note However values of fact parameter are reasonable only in interval
		* [0.0 , 1.0], you can pass also values outside of this interval and you
		* can get result (extrapolation?)
		*/
		Matrix4<T> lerp(T fact, const Matrix4<T>& rhs) const
		{
			Matrix4<T> ret = (*this) + (rhs - (*this)) * fact;
			return ret;
		}

		//-------------[ conversion ]-----------------------------
		/**
		* Conversion to pointer operator
		* @return Pointer to internally stored (in management of class Matrix4<T>)
		* used for passing Matrix4<T> values to gl*[fd]v functions.
		*/
		operator T*()
		{
			return (T*)data;
		}

		/**
		* Conversion to pointer operator
		* @return Constant Pointer to internally stored (in management of class Matrix4<T>)
		* used for passing Matrix4<T> values to gl*[fd]v functions.
		*/
		operator const T*() const
		{
			return (const T*)data;
		}

		//----------[ output operator ]----------------------------
		/**
		* Output to stream operator
		* @param lhs Left hand side argument of operator (commonly ostream instance).
		* @param rhs Right hand side argument of operator.
		* @return Left hand side argument - the ostream object passed to operator.
		*/
		friend std::ostream& operator <<(std::ostream& lhs, const Matrix4<T>& rhs)
		{
			for (int i = 0; i < 4; i++)
			{
				lhs << "|\t";
				for (int j = 0; j < 4; j++)
				{
					lhs << rhs.at(j, i) << "\t";
				}
				lhs << "|" << std::endl;
			}
			return lhs;
		}

		/**
		* Gets string representation.
		*/
		std::string toString() const
		{
			std::ostringstream oss;
			oss << *this;
			return oss.str();
		}

	};

	/// Matrix 4x4 of floats
	typedef Matrix4<float> Matrix4f;
	/// Matrix 4x4 of doubles
	typedef Matrix4<double> Matrix4d;
	/// Matrix 4x4 of int
	typedef Matrix4<int> Matrix4i;

};