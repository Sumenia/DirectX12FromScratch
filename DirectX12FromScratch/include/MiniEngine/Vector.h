#pragma once

#include <cmath>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

//#define DEG2RAD(x) ((x * M_PI) / 180.0)

namespace MiniEngine
{
	template<class T>
	class Vector2
	{
	public:
		union
		{
			/**
			* First element of vector, alias for X-coordinate.
			*/
			T x;

			/**
			* First element of vector, alias for S-coordinate.
			* For textures notation.
			*/
			T s;
		};

		union
		{
			/**
			* Second element of vector, alias for Y-coordinate.
			*/
			T y;

			/**
			* Second element of vector, alias for T-coordinate.
			* For textures notation.
			*/
			T t;
		};

		//----------------[ constructors ]--------------------------
		/**
		* Creates and sets to (0,0)
		*/
		Vector2()
			: x(0), y(0)
		{
		}

		/**
		* Creates and sets to (x,y)
		* @param nx initial x-coordinate value
		* @param ny initial y-coordinate value
		*/
		Vector2(T nx, T ny)
			: x(nx), y(ny)
		{
		}

		/**
		* Copy constructor.
		* @param src Source of data for new created instance.
		*/
		Vector2(const Vector2<T>& src)
			: x(src.x), y(src.y)
		{
		}

		/**
		* Copy casting constructor.
		* @param src Source of data for new created instance.
		*/
		template<class FromT>
		Vector2(const Vector2<FromT>& src)
			: x(static_cast<T>(src.x)), y(static_cast<T>(src.y))
		{
		}

		//----------------[ access operators ]-------------------
		/**
		* Copy casting operator
		* @param rhs Right hand side argument of binary operator.
		*/
		template<class FromT>
		Vector2<T>& operator=(const Vector2<FromT>& rhs)
		{
			x = static_cast<T>(rhs.x);
			y = static_cast<T>(rhs.y);
			return *this;
		}

		/**
		* Copy operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T>& operator=(const Vector2<T>& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			return *this;
		}

		/**
		* Array access operator
		* @param n Array index
		* @return For n = 0, reference to x coordinate, else reference to y
		* y coordinate.
		*/
		T& operator[](int n)
		{
			assert(n >= 0 && n <= 1);
			if (0 == n)
				return x;
			else
				return y;
		}

		/**
		* Constant array access operator
		* @param n Array index
		* @return For n = 0, reference to x coordinate, else reference to y
		* y coordinate.
		*/
		const T& operator[](int n) const
		{
			assert(n >= 0 && n <= 1);
			if (0 == n)
				return x;
			else
				return y;
		}

		//---------------[ vector aritmetic operator ]--------------
		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T> operator+(const Vector2<T>& rhs) const
		{
			return Vector2<T>(x + rhs.x, y + rhs.y);
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T> operator-(const Vector2<T>& rhs) const
		{
			return Vector2<T>(x - rhs.x, y - rhs.y);
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T> operator*(const Vector2<T>& rhs) const
		{
			return Vector2<T>(x * rhs.x, y * rhs.y);
		}

		/**
		* Division operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T> operator/(const Vector2<T>& rhs) const
		{
			return Vector2<T>(x / rhs.x, y / rhs.y);
		}

		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T>& operator+=(const Vector2<T>& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		/**
		* Substraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T>& operator-=(const Vector2<T>& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T>& operator*=(const Vector2<T>& rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
			return *this;
		}

		/**
		* Division operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T>& operator/=(const Vector2<T>& rhs)
		{
			x /= rhs.x;
			y /= rhs.y;
			return *this;
		}

		//--------------[ scalar vector operator ]--------------------
		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T> operator+(T rhs) const
		{
			return Vector2<T>(x + rhs, y + rhs);
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T> operator-(T rhs) const
		{
			return Vector2<T>(x - rhs, y - rhs);
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T> operator*(T rhs) const
		{
			return Vector2<T>(x * rhs, y * rhs);
		}

		/**
		* Division operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T> operator/(T rhs) const
		{
			return Vector2<T>(x / rhs, y / rhs);
		}

		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T>& operator+=(T rhs)
		{
			x += rhs;
			y += rhs;
			return *this;
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T>& operator-=(T rhs)
		{
			x -= rhs;
			y -= rhs;
			return *this;
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T>& operator*=(T rhs)
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}

		/**
		* Division operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector2<T>& operator/=(T rhs)
		{
			x /= rhs;
			y /= rhs;
			return *this;
		}

		//--------------[ equality operator ]------------------------
		/**
		* Equality test operator
		* @param rhs Right hand side argument of binary operator.
		* @note Test of equality is based of threshold EPSILON value. To be two
		* values equal, must satisfy this condition | lhs.x - rhs.y | < EPSILON,
		* same for y-coordinate.
		*/
		bool operator==(const Vector2<T>& rhs) const
		{
			return (std::abs(x - rhs.x) < FLT_EPSILON) && (std::abs(y - rhs.y) < FLT_EPSILON);
		}

		/**
		* Inequality test operator
		* @param rhs Right hand side argument of binary operator.
		* @return not (lhs == rhs) :-P
		*/
		bool operator!=(const Vector2<T>& rhs) const
		{
			return !(*this == rhs);
		}

		//-------------[ unary operations ]--------------------------
		/**
		* Unary negate operator
		* @return negated vector
		*/
		Vector2<T> operator-() const
		{
			return Vector2<T>(-x, -y);
		}

		//-------------[ size operations ]---------------------------
		/**
		* Get length of vector.
		* @return lenght of vector
		*/
		T length() const
		{
			return (T)std::sqrt(x * x + y * y);
		}

		/**
		* Normalize vector
		*/
		void normalize()
		{
			T s = length();
			x /= s;
			y /= s;
		}

		/**
		* Return square of length.
		* @return length ^ 2
		* @note This method is faster then length(). For comparison
		* of length of two vector can be used just this value, instead
		* of more expensive length() method.
		*/
		T lengthSq() const
		{
			return x * x + y * y;
		}

		//--------------[ misc. operations ]-----------------------
		/**
		* Linear interpolation of two vectors
		* @param fact Factor of interpolation. For translation from position
		* of this vector to vector r, values of factor goes from 0.0 to 1.0.
		* @param r Second Vector for interpolation
		* @note However values of fact parameter are reasonable only in interval
		* [0.0 , 1.0], you can pass also values outside of this interval and you
		* can get result (extrapolation?)
		*/
		Vector2<T> lerp(T fact, const Vector2<T>& r) const
		{
			return (*this) + (r - (*this)) * fact;
		}

		//-------------[ conversion ]-----------------------------
		/**
		* Conversion to pointer operator
		* @return Pointer to internally stored (in management of class Vector2<T>)
		* used for passing Vector2<T> values to gl*2[fd] functions.
		*/
		operator T*()
		{
			return (T*) this;
		}
		/**
		* Conversion to pointer operator
		* @return Constant Pointer to internally stored (in management of class Vector2<T>)
		* used for passing Vector2<T> values to gl*2[fd] functions.
		*/
		operator const T*() const
		{
			return (const T*) this;
		}

		//-------------[ output operator ]------------------------
		/**
		* Output to stream operator
		* @param lhs Left hand side argument of operator (commonly ostream instance).
		* @param rhs Right hand side argument of operator.
		* @return Left hand side argument - the ostream object passed to operator.
		*/
		friend std::ostream& operator<<(std::ostream& lhs, const Vector2<T>& rhs)
		{
			lhs << "[" << rhs.x << "," << rhs.y << "]";
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

	//--------------------------------------
	// Typedef shortcuts for 2D vector
	//-------------------------------------
	/// Two dimensional Vector of floats
	typedef class Vector2<float> Vector2f;
	/// Two dimensional Vector of doubles
	typedef class Vector2<double> Vector2d;
	/// Two dimensional Vector of ints
	typedef class Vector2<int> Vector2i;

	/**
	* Class for three dimensional vector.
	* There are four ways of accessing vector components.
	* Let's have <code>Vector3f v</code>, you can either:
	* <ul>
	* 	<li>access as position (x,y,z) &mdash; <code>v.x = v.y = v.z = 1;</code></li>
	* 	<li>access as texture coordinate (s,t,u) &mdash; <code>v.s = v.t = v.u = 1;</code></li>
	* 	<li>access as color (r,g,b) &mdash; <code>v.r = v.g = v.b = 1;</code></li>
	* 	<li>access via operator[] &mdash; <code>v[0] = v[1] = v[2] = 1;</code></li>
	* </ul>
	*/
	template<class T>
	class Vector3
	{
	public:
		//T x, y, z;
		union
		{
			/**
			* First element of vector, alias for X-coordinate.
			*/
			T x;

			/**
			* First element of vector, alias for S-coordinate.
			* For textures notation.
			*/
			T s;

			/**
			* First element of vector, alias for R-coordinate.
			* For color notation.
			*/
			T r;
		};

		union
		{
			/**
			* Second element of vector, alias for Y-coordinate.
			*/
			T y;
			/**
			* Second element of vector, alias for T-coordinate.
			* For textures notation.
			*/
			T t;
			/**
			* Second element of vector, alias for G-coordinate.
			* For color notation.
			*/
			T g;
		};

		union
		{
			/**
			* Third element of vector, alias for Z-coordinate.
			*/
			T z;

			/**
			* Third element of vector, alias for U-coordinate.
			* For textures notation.
			*/
			T u;
			/**
			* Third element of vector, alias for B-coordinate.
			* For color notation.
			*/
			T b;
		};

		//----------------[ constructors ]--------------------------
		/**
		* Creates and sets to (0,0,0)
		*/
		Vector3()
			: x(0), y(0), z(0)
		{
		}

		/**
		* Creates and sets to (x,y,z)
		* @param nx initial x-coordinate value
		* @param ny initial y-coordinate value
		* @param nz initial z-coordinate value
		*/
		Vector3(T nx, T ny, T nz)
			: x(nx), y(ny), z(nz)
		{
		}

		/**
		* Copy constructor.
		* @param src Source of data for new created Vector3 instance.
		*/
		Vector3(const Vector3<T>& src)
			: x(src.x), y(src.y), z(src.z)
		{
		}

		/**
		* Copy casting constructor.
		* @param src Source of data for new created Vector3 instance.
		*/
		template<class FromT>
		Vector3(const Vector3<FromT>& src)
			: x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z))
		{
		}

		//----------------[ access operators ]-------------------
		/**
		* Copy operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T> operator=(const Vector3<T>& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			return *this;
		}

		/**
		* Copy casting operator.
		* @param rhs Right hand side argument of binary operator.
		*/
		template<class FromT>
		Vector3<T> operator=(const Vector3<FromT>& rhs)
		{
			x = static_cast<T>(rhs.x);
			y = static_cast<T>(rhs.y);
			z = static_cast<T>(rhs.z);
			return *this;
		}

		/**
		* Array access operator
		* @param n Array index
		* @return For n = 0, reference to x coordinate, n = 1
		* reference to y, else reference to z
		* y coordinate.
		*/
		T & operator[](int n)
		{
			assert(n >= 0 && n <= 2);
			if (0 == n)
				return x;
			else if (1 == n)
				return y;
			else
				return z;
		}

		/**
		* Constant array access operator
		* @param n Array index
		* @return For n = 0, reference to x coordinate, n = 1
		* reference to y, else reference to z
		* y coordinate.
		*/
		const T & operator[](int n) const
		{
			assert(n >= 0 && n <= 2);
			if (0 == n)
				return x;
			else if (1 == n)
				return y;
			else
				return z;
		}

		//---------------[ vector arithmetic operator ]--------------
		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T> operator+(const Vector3<T>& rhs) const
		{
			return Vector3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T> operator-(const Vector3<T>& rhs) const
		{
			return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T> operator*(const Vector3<T>& rhs) const
		{
			return Vector3<T>(x * rhs.x, y * rhs.y, z * rhs.z);
		}

		/**
		* Division operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T> operator/(const Vector3<T>& rhs) const
		{
			return Vector3<T>(x / rhs.x, y / rhs.y, z / rhs.z);
		}

		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T>& operator+=(const Vector3<T>& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T>& operator-=(const Vector3<T>& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T>& operator*=(const Vector3<T>& rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
			return *this;
		}

		/**
		* Division operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T>& operator/=(const Vector3<T>& rhs)
		{
			x /= rhs.x;
			y /= rhs.y;
			z /= rhs.z;
			return *this;
		}

		/**
		* Dot product of two vectors.
		* @param rhs Right hand side argument of binary operator.
		*/
		T dotProduct(const Vector3<T>& rhs) const
		{
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}

		/**
		* Cross product operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T> crossProduct(const Vector3<T>& rhs) const
		{
			return Vector3<T>(y * rhs.z - rhs.y * z, z * rhs.x - rhs.z * x, x * rhs.y - rhs.x * y);
		}

		//--------------[ scalar vector operator ]--------------------
		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T> operator+(T rhs) const
		{
			return Vector3<T>(x + rhs, y + rhs, z + rhs);
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T> operator-(T rhs) const
		{
			return Vector3<T>(x - rhs, y - rhs, z - rhs);
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T> operator*(T rhs) const
		{
			return Vector3<T>(x * rhs, y * rhs, z * rhs);
		}

		/**
		* Division operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T> operator/(T rhs) const
		{
			return Vector3<T>(x / rhs, y / rhs, z / rhs);
		}

		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T>& operator+=(T rhs)
		{
			x += rhs;
			y += rhs;
			z += rhs;
			return *this;
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T>& operator-=(T rhs)
		{
			x -= rhs;
			y -= rhs;
			z -= rhs;
			return *this;
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T>& operator*=(T rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}

		/**
		* Division operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector3<T>& operator/=(T rhs)
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
			return *this;
		}

		//--------------[ Equality operator ]------------------------
		/**
		* Equality test operator
		* @param rhs Right hand side argument of binary operator.
		* @note Test of equality is based of threshold EPSILON value. To be two
		* values equal, must satisfy this condition | lhs.x - rhs.y | < EPSILON,
		* same for y-coordinate, and z-coordinate.
		*/
		bool operator==(const Vector3<T>& rhs) const
		{
			return std::fabs(x - rhs.x) < EPSILON && std::fabs(y - rhs.y) < EPSILON && std::fabs(z - rhs.z) < EPSILON;
		}

		/**
		* Inequality test operator
		* @param rhs Right hand side argument of binary operator.
		* @return not (lhs == rhs) :-P
		*/
		bool operator!=(const Vector3<T>& rhs) const
		{
			return !(*this == rhs);
		}

		//-------------[ unary operations ]--------------------------
		/**
		* Unary negate operator
		* @return negated vector
		*/
		Vector3<T> operator-() const
		{
			return Vector3<T>(-x, -y, -z);
		}

		//-------------[ size operations ]---------------------------
		/**
		* Get length of vector.
		* @return lenght of vector
		*/
		T length() const
		{
			return (T)std::sqrt(x * x + y * y + z * z);
		}

		/**
		* Return square of length.
		* @return length ^ 2
		* @note This method is faster then length(). For comparison
		* of length of two vector can be used just this value, instead
		* of more expensive length() method.
		*/
		T lengthSq() const
		{
			return x * x + y * y + z * z;
		}

		/**
		* Normalize vector
		*/
		void normalize()
		{
			T s = length();
			x /= s;
			y /= s;
			z /= s;
		}

		//------------[ other operations ]---------------------------
		/**
		* Rotate vector around three axis.
		* @param ax Angle (in degrees) to be rotated around X-axis.
		* @param ay Angle (in degrees) to be rotated around Y-axis.
		* @param az Angle (in degrees) to be rotated around Z-axis.
		*/
		void rotate(T ax, T ay, T az)
		{
			T a = cos(DEG2RAD(ax));
			T b = sin(DEG2RAD(ax));
			T c = cos(DEG2RAD(ay));
			T d = sin(DEG2RAD(ay));
			T e = cos(DEG2RAD(az));
			T f = sin(DEG2RAD(az));
			T nx = c * e * x - c * f * y + d * z;
			T ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
			T nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
			x = nx;
			y = ny;
			z = nz;

		}

		/**
		* Linear interpolation of two vectors
		* @param fact Factor of interpolation. For translation from positon
		* of this vector to vector r, values of factor goes from 0.0 to 1.0.
		* @param r Second Vector for interpolation
		* @note However values of fact parameter are reasonable only in interval
		* [0.0 , 1.0], you can pass also values outside of this interval and you
		* can get result (extrapolation?)
		*/
		Vector3<T> lerp(T fact, const Vector3<T>& r) const
		{
			return (*this) + (r - (*this)) * fact;
		}

		//-------------[ conversion ]-----------------------------

		/**
		* Conversion to pointer operator
		* @return Pointer to internally stored (in management of class Vector3<T>)
		* used for passing Vector3<T> values to gl*3[fd] functions.
		*/
		operator T*()
		{
			return (T*) this;
		}

		/**
		* Conversion to pointer operator
		* @return Constant Pointer to internally stored (in management of class Vector3<T>)
		* used for passing Vector3<T> values to gl*3[fd] functions.
		*/
		operator const T*() const
		{
			return (const T*) this;
		}

		//-------------[ output operator ]------------------------
		/**
		* Output to stream operator
		* @param lhs Left hand side argument of operator (commonly ostream instance).
		* @param rhs Right hand side argument of operator.
		* @return Left hand side argument - the ostream object passed to operator.
		*/
		friend std::ostream& operator<<(std::ostream& lhs, const Vector3<T> rhs)
		{
			lhs << "[" << rhs.x << "," << rhs.y << "," << rhs.z << "]";
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

	/// Three dimensional Vector of floats
	typedef Vector3<float> Vector3f;
	/// Three dimensional Vector of doubles
	typedef Vector3<double> Vector3d;
	/// Three dimensional Vector of ints
	typedef Vector3<int> Vector3i;

	/**
	* Class for four dimensional vector.
	* There are four ways of accessing vector components.
	* Let's have <code>Vector4f v</code>, you can either:
	* <ul>
	* 	<li>access as position in projective space (x,y,z,w) &mdash; <code>v.x = v.y = v.z = v.w = 1;</code></li>
	* 	<li>access as texture coordinate (s,t,u,v) &mdash; <code>v.s = v.t = v.u = v.v = 1;</code></li>
	* 	<li>access as color (r,g,b,a) &mdash; <code>v.r = v.g = v.b = v.a = 1;</code></li>
	* 	<li>access via operator[] &mdash; <code>v[0] = v[1] = v[2] = v[3] = 1;</code></li>
	* </ul>
	*/
	template<class T>
	class Vector4
	{
	public:

		union
		{
			/**
			* First element of vector, alias for R-coordinate.
			* For color notation.
			*/
			T r
				/**
				* First element of vector, alias for X-coordinate.
				*/;
			T x;
		};

		union
		{
			/**
			* Second element of vector, alias for G-coordinate.
			* For color notation.
			*/
			T g;
			/**
			* Second element of vector, alias for Y-coordinate.
			*/
			T y;
		};

		union
		{
			/**
			* Third element of vector, alias for B-coordinate.
			* For color notation.
			*/
			T b;
			/**
			* Third element of vector, alias for Z-coordinate.
			*/
			T z;
		};

		union
		{
			/**
			* Fourth element of vector, alias for A-coordinate.
			* For color notation. This represnt aplha chanell
			*/
			T a;
			/**
			* First element of vector, alias for W-coordinate.
			* @note For vectors (such as normals) should be set to 0.0
			* For vertices should be set to 1.0
			*/
			T w;
		};

		//----------------[ constructors ]--------------------------
		/**
		* Creates and sets to (0,0,0,0)
		*/
		Vector4()
			: x(0), y(0), z(0), w(0)
		{
		}

		/**
		* Creates and sets to (x,y,z,z)
		* @param nx initial x-coordinate value (R)
		* @param ny initial y-coordinate value (G)
		* @param nz initial z-coordinate value (B)
		* @param nw initial w-coordinate value (Alpha)
		*/
		Vector4(T nx, T ny, T nz, T nw)
			: x(nx), y(ny), z(nz), w(nw)
		{
		}

		/**
		* Copy constructor.
		* @param src Source of data for new created Vector4 instance.
		*/
		Vector4(const Vector4<T>& src)
			: x(src.x), y(src.y), z(src.z), w(src.w)
		{
		}

		/**
		* Copy casting constructor.
		* @param src Source of data for new created Vector4 instance.
		*/
		template<class FromT>
		Vector4(const Vector4<FromT>& src)
			: x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z)), w(static_cast<T>(src.w))
		{
		}

		Vector4(const Vector3<T>& src, T w)
			: x(src.x), y(src.y), z(src.z), w(w)
		{}

		template <typename FromT>
		Vector4(const Vector3<FromT>& src, FromT w)
			: x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z)), w(static_cast<T>(w))
		{}

		//----------------[ access operators ]-------------------
		/**
		* Copy operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T> operator=(const Vector4<T>& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
			return *this;
		}

		/**
		* Copy casting operator
		* @param rhs Right hand side argument of binary operator.
		*/
		template<class FromT>
		Vector4<T> operator=(const Vector4<FromT>& rhs)
		{
			x = static_cast<T>(rhs.x);
			y = static_cast<T>(rhs.y);
			z = static_cast<T>(rhs.z);
			w = static_cast<T>(rhs.w);
			return *this;
		}

		/**
		* Array access operator
		* @param n Array index
		* @return For n = 0, reference to x coordinate, n = 1
		* reference to y coordinate, n = 2 reference to z,
		* else reference to w coordinate.
		*/
		T & operator[](int n)
		{
			assert(n >= 0 && n <= 3);
			if (0 == n)
				return x;
			else if (1 == n)
				return y;
			else if (2 == n)
				return z;
			else
				return w;
		}

		/**
		* Array access operator
		* @param n Array index
		* @return For n = 0, reference to x coordinate, n = 1
		* reference to y coordinate, n = 2 reference to z,
		* else reference to w coordinate.
		*/
		const T & operator[](int n) const
		{
			assert(n >= 0 && n <= 3);
			if (0 == n)
				return x;
			else if (1 == n)
				return y;
			else if (2 == n)
				return z;
			else
				return w;
		}

		//---------------[ vector aritmetic operator ]--------------
		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T> operator+(const Vector4<T>& rhs) const
		{
			return Vector4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T> operator-(const Vector4<T>& rhs) const
		{
			return Vector4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T> operator*(const Vector4<T> rhs) const
		{
			return Vector4<T>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
		}

		/**
		* Division operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T> operator/(const Vector4<T>& rhs) const
		{
			return Vector4<T>(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
		}

		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T>& operator+=(const Vector4<T>& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
			return *this;
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T>& operator-=(const Vector4<T>& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
			return *this;
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T>& operator*=(const Vector4<T>& rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
			w *= rhs.w;
			return *this;
		}

		/**
		* Division operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T>& operator/=(const Vector4<T>& rhs)
		{
			x /= rhs.x;
			y /= rhs.y;
			z /= rhs.z;
			w /= rhs.w;
			return *this;
		}

		//--------------[ equiality operator ]------------------------
		/**
		* Equality test operator
		* @param rhs Right hand side argument of binary operator.
		* @note Test of equality is based of threshold EPSILON value. To be two
		* values equal, must satisfy this condition | lhs.x - rhs.y | < EPSILON,
		* same for y-coordinate, z-coordinate, and w-coordinate.
		*/
		bool operator==(const Vector4<T>& rhs) const
		{
			return std::fabs(x - rhs.x) < EPSILON && std::fabs(y - rhs.y) < EPSILON && std::fabs(z - rhs.z) < EPSILON
				&& std::fabs(w - rhs.w) < EPSILON;
		}

		/**
		* Inequality test operator
		* @param rhs Right hand side argument of binary operator.
		* @return not (lhs == rhs) :-P
		*/
		bool operator!=(const Vector4<T>& rhs) const
		{
			return !(*this == rhs);
		}

		//-------------[ unary operations ]--------------------------
		/**
		* Unary negate operator
		* @return negated vector
		*/
		Vector4<T> operator-() const
		{
			return Vector4<T>(-x, -y, -z, -w);
		}

		//--------------[ scalar vector operator ]--------------------

		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T> operator+(T rhs) const
		{
			return Vector4<T>(x + rhs, y + rhs, z + rhs, w + rhs);
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T> operator-(T rhs) const
		{
			return Vector4<T>(x - rhs, y - rhs, z - rhs, w - rhs);
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T> operator*(T rhs) const
		{
			return Vector4<T>(x * rhs, y * rhs, z * rhs, w * rhs);
		}

		/**
		* Division operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T> operator/(T rhs) const
		{
			return Vector4<T>(x / rhs, y / rhs, z / rhs, w / rhs);
		}

		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T>& operator+=(T rhs)
		{
			x += rhs;
			y += rhs;
			z += rhs;
			w += rhs;
			return *this;
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T>& operator-=(T rhs)
		{
			x -= rhs;
			y -= rhs;
			z -= rhs;
			w -= rhs;
			return *this;
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T>& operator*=(T rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			w *= rhs;
			return *this;
		}

		/**
		* Division operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Vector4<T>& operator/=(T rhs)
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
			w /= rhs;
			return *this;
		}

		//-------------[ size operations ]---------------------------
		/**
		* Get length of vector.
		* @return lenght of vector
		*/
		T length() const
		{
			return (T)std::sqrt(x * x + y * y + z * z + w * w);
		}

		/**
		* Normalize vector
		*/
		void normalize()
		{
			T s = length();
			x /= s;
			y /= s;
			z /= s;
			w /= s;
		}

		/**
		* Return square of length.
		* @return length ^ 2
		* @note This method is faster then length(). For comparison
		* of length of two vector can be used just this value, instead
		* of more expensive length() method.
		*/
		T lengthSq() const
		{
			return x * x + y * y + z * z + w * w;
		}

		//--------------[ misc. operations ]-----------------------
		/**
		* Linear interpolation of two vectors
		* @param fact Factor of interpolation. For translation from position
		* of this vector to vector r, values of factor goes from 0.0 to 1.0.
		* @param r Second Vector for interpolation
		* @note However values of fact parameter are reasonable only in interval
		* [0.0 , 1.0], you can pass also values outside of this interval and you
		* can get result (extrapolation?)
		*/
		Vector4<T> lerp(T fact, const Vector4<T>& r) const
		{
			return (*this) + (r - (*this)) * fact;
		}

		//-------------[ conversion ]-----------------------------

		/**
		* Conversion to pointer operator
		* @return Pointer to internally stored (in management of class Vector4<T>)
		* used for passing Vector4<T> values to gl*4[fd] functions.
		*/
		operator T*()
		{
			return (T*) this;
		}

		/**
		* Conversion to pointer operator
		* @return Constant Pointer to internally stored (in management of class Vector4<T>)
		* used for passing Vector4<T> values to gl*4[fd] functions.
		*/
		operator const T*() const
		{
			return (const T*) this;
		}

		/**
		* Gets 3D vector. Note that the output is divided by w coordinate to apply projection
		* transform. If the w coordinate is equal to zero, the result is not divided.
		* @return (x/w, y/w, z/w) iff w != 0 otherwise (x,y,z)
		*/
		Vector3<T> xyz() const
		{
			if (w == 0 || w == 1)
				return Vector3<T>(x, y, z);

			const T invW = 1.0 / w;
			return Vector3<T>(x * invW, y * invW, z * invW);
		}

		//-------------[ output operator ]------------------------
		/**
		* Output to stream operator
		* @param lhs Left hand side argument of operator (commonly ostream instance).
		* @param rhs Right hand side argument of operator.
		* @return Left hand side argument - the ostream object passed to operator.
		*/
		friend std::ostream& operator<<(std::ostream& lhs, const Vector4<T>& rhs)
		{
			lhs << "[" << rhs.x << "," << rhs.y << "," << rhs.z << "," << rhs.w << "]";
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

	/// Three dimensional Vector of floats
	typedef Vector4<float> Vector4f;
	/// Three dimensional Vector of doubles
	typedef Vector4<double> Vector4d;
	/// Three dimensional Vector of ints
	typedef Vector4<int> Vector4i;
};