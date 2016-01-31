#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

#ifndef DEG2RAD
# define DEG2RAD(x) ((x * M_PI) / 180.0)
#endif

namespace MiniEngine
{
	template<class T>
	class Quaternion
	{
	public:
		/**
		* Real part of quaternion.
		*/
		T w;
		/**
		* Imaginary part of quaternion.
		*/
		Vector3<T> v;

		/**
		* Quaternion constructor, sets quaternion to (0 + 0i + 0j + 0k).
		*/
		Quaternion()
			: w(0), v(0, 0, 0)
		{
		}

		/**
		* Copy constructor.
		*/
		Quaternion(const Quaternion<T>& q)
			: w(q.w), v(q.v)
		{
		}

		/**
		* Copy casting constructor.
		*/
		template<class FromT>
		Quaternion(const Quaternion<FromT>& q)
			: w(static_cast<T>(q.w)), v(q.v)
		{
		}

		/**
		* Creates quaternion object from real part w_ and complex part v_.
		* @param w_ Real part of quaternion.
		* @param v_ Complex part of quaternion (xi + yj + zk).
		*/
		Quaternion(T w_, const Vector3<T>& v_)
			: w(w_), v(v_)
		{
		}

		/**
		* Creates quaternion object from value (w_ + xi + yj + zk).
		* @param w_ Real part of quaternion.
		* @param x Complex coefficient for i complex constant.
		* @param y Complex coefficient for j complex constant.
		* @param z Complex coefficient for k complex constant.
		*/
		Quaternion(T w_, T x, T y, T z)
			: w(w_), v(x, y, z)
		{
		}

		/**
		* Copy operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Quaternion<T>& operator=(const Quaternion<T>& rhs)
		{
			v = rhs.v;
			w = rhs.w;
			return *this;
		}

		/**
		* Copy convert operator
		* @param rhs Right hand side argument of binary operator.
		*/
		template<class FromT>
		Quaternion<T>& operator=(const Quaternion<FromT>& rhs)
		{
			v = rhs.v;
			w = static_cast<T>(rhs.w);
			return *this;
		}

		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Quaternion<T> operator+(const Quaternion<T>& rhs) const
		{
			const Quaternion<T>& lhs = *this;
			return Quaternion<T>(lhs.w + rhs.w, lhs.v + rhs.v);
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Quaternion<T> operator*(const Quaternion<T>& rhs) const
		{
			const Quaternion<T>& lhs = *this;
			return Quaternion<T>(lhs.w * rhs.w - lhs.v.x * rhs.v.x - lhs.v.y * rhs.v.y - lhs.v.z * rhs.v.z,
				lhs.w * rhs.v.x + lhs.v.x * rhs.w + lhs.v.y * rhs.v.z - lhs.v.z * rhs.v.y,
				lhs.w * rhs.v.y - lhs.v.x * rhs.v.z + lhs.v.y * rhs.w + lhs.v.z * rhs.v.x,
				lhs.w * rhs.v.z + lhs.v.x * rhs.v.y - lhs.v.y * rhs.v.x + lhs.v.z * rhs.w);
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Quaternion<T> operator*(T rhs) const
		{
			return Quaternion<T>(w * rhs, v * rhs);
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Quaternion<T> operator-(const Quaternion<T>& rhs) const
		{
			const Quaternion<T>& lhs = *this;
			return Quaternion<T>(lhs.w - rhs.w, lhs.v - rhs.v);
		}

		/**
		* Addition operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Quaternion<T>& operator+=(const Quaternion<T>& rhs)
		{
			w += rhs.w;
			v += rhs.v;
			return *this;
		}

		/**
		* Subtraction operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Quaternion<T>& operator-=(const Quaternion<T>& rhs)
		{
			w -= rhs.w;
			v -= rhs.v;
			return *this;
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Quaternion<T>& operator*=(const Quaternion<T>& rhs)
		{
			Quaternion q = (*this) * rhs;
			v = q.v;
			w = q.w;
			return *this;
		}

		/**
		* Multiplication operator
		* @param rhs Right hand side argument of binary operator.
		*/
		Quaternion<T>& operator*=(T rhs)
		{
			w *= rhs;
			v *= rhs;
			return *this;
		}

		/**
		* Equality test operator
		* @param rhs Right hand side argument of binary operator.
		* @note Test of equality is based of threshold EPSILON value. To be two
		* values equal, must satisfy this condition | lhs - rhs | < EPSILON,
		* for all quaternion coordinates.
		*/
		bool operator==(const Quaternion<T>& rhs) const
		{
			const Quaternion<T>& lhs = *this;
			return (std::fabs(lhs.w - rhs.w) < EPSILON) && lhs.v == rhs.v;
		}

		/**
		* Inequality test operator
		* @param rhs Right hand side argument of binary operator.
		* @return not (lhs == rhs) :-P
		*/
		bool operator!=(const Quaternion<T>& rhs) const
		{
			return !(*this == rhs);
		}

		//-------------[ unary operations ]--------------------------
		/**
		* Unary negate operator
		* @return negated quaternion
		*/
		Quaternion<T> operator-() const
		{
			return Quaternion<T>(-w, -v);
		}

		/**
		* Unary conjugate operator
		* @return conjugated quaternion
		*/
		Quaternion<T> operator~() const
		{
			return Quaternion<T>(w, -v);
		}

		/**
		* Get lenght of quaternion.
		* @return Length of quaternion.
		*/
		T length() const
		{
			return (T)std::sqrt(w * w + v.lengthSq());
		}

		/**
		* Return square of length.
		* @return length ^ 2
		* @note This method is faster then length(). For comparison
		* of length of two quaternion can be used just this value, instead
		* of more expensive length() method.
		*/
		T lengthSq() const
		{
			return w * w + v.lengthSq();
		}

		/**
		* Normalize quaternion
		*/
		void normalize()
		{
			T len = length();
			w /= len;
			v /= len;
		}

		/**
		* Creates quaternion for eulers angles.
		* @param x Rotation around x axis (in degrees).
		* @param y Rotation around y axis (in degrees).
		* @param z Rotation around z axis (in degrees).
		* @return Quaternion object representing transformation.
		*/
		static Quaternion<T> fromEulerAngles(T x, T y, T z)
		{
			Quaternion<T> ret = fromAxisRot(Vector3<T>(1, 0, 0), x) * fromAxisRot(Vector3<T>(0, 1, 0), y)
				* fromAxisRot(Vector3<T>(0, 0, 1), z);
			return ret;
		}

		/**
		* Creates quaternion as rotation around axis.
		* @param axis Unit vector expressing axis of rotation.
		* @param angleDeg Angle of rotation around axis (in degrees).
		*/
		static Quaternion<T> fromAxisRot(Vector3<T> axis, float angleDeg)
		{
			double angleRad = DEG2RAD(angleDeg);
			double sa2 = std::sin(angleRad / 2);
			double ca2 = std::cos(angleRad / 2);
			return Quaternion<T>(ca2, axis * sa2);
		}

		/**
		* Converts quaternion into rotation matrix.
		* @return Rotation matrix expressing this quaternion.
		*/
		Matrix3<T> rotMatrix()
		{
			Matrix3<T> ret;

			/*ret.at(0,0) = 1 - 2*v.y*v.y - 2*v.z*v.z;
			ret.at(1,0) = 2*v.x*v.y - 2*w*v.z;
			ret.at(2,0) = 2*v.x*v.z - 2*w*v.y;

			ret.at(0,1) = 2*v.x*v.y + 2*w*v.z;
			ret.at(1,1) = 1 - 2*v.x*v.x - 2*v.z*v.z;
			ret.at(2,1) = 2*v.y*v.z - 2*w*v.x;

			ret.at(0,2) = 2*v.x*v.z - 2*w*v.y;
			ret.at(1,2) = 2*v.y*v.z + 2*w*v.x;
			ret.at(2,2) = 1 - 2*v.x*v.x - 2*v.y*v.y;*/

			T xx = v.x * v.x;
			T xy = v.x * v.y;
			T xz = v.x * v.z;
			T xw = v.x * w;

			T yy = v.y * v.y;
			T yz = v.y * v.z;
			T yw = v.y * w;

			T zz = v.z * v.z;
			T zw = v.z * w;

			ret.at(0, 0) = 1 - 2 * (yy + zz);
			ret.at(1, 0) = 2 * (xy - zw);
			ret.at(2, 0) = 2 * (xz + yw);

			ret.at(0, 1) = 2 * (xy + zw);
			ret.at(1, 1) = 1 - 2 * (xx + zz);
			ret.at(2, 1) = 2 * (yz - xw);

			ret.at(0, 2) = 2 * (xz - yw);
			ret.at(1, 2) = 2 * (yz + xw);
			ret.at(2, 2) = 1 - 2 * (xx + yy);

			return ret;
		}

		/**
		* Converts quaternion into transformation matrix.
		* @note This method performs same operation as rotMatrix()
		* conversion method. But returns Matrix of 4x4 elements.
		* @return Transformation matrix expressing this quaternion.
		*/
		Matrix4<T> transform() const
		{
			Matrix4<T> ret;

			T xx = v.x * v.x;
			T xy = v.x * v.y;
			T xz = v.x * v.z;
			T xw = v.x * w;

			T yy = v.y * v.y;
			T yz = v.y * v.z;
			T yw = v.y * w;

			T zz = v.z * v.z;
			T zw = v.z * w;

			ret.at(0, 0) = 1 - 2 * (yy + zz);
			ret.at(1, 0) = 2 * (xy - zw);
			ret.at(2, 0) = 2 * (xz + yw);
			ret.at(3, 0) = 0;

			ret.at(0, 1) = 2 * (xy + zw);
			ret.at(1, 1) = 1 - 2 * (xx + zz);
			ret.at(2, 1) = 2 * (yz - xw);
			ret.at(3, 1) = 0;

			ret.at(0, 2) = 2 * (xz - yw);
			ret.at(1, 2) = 2 * (yz + xw);
			ret.at(2, 2) = 1 - 2 * (xx + yy);
			ret.at(3, 2) = 0;

			ret.at(0, 3) = 0;
			ret.at(1, 3) = 0;
			ret.at(2, 3) = 0;
			ret.at(3, 3) = 1;

			return ret;

		}

		/**
		* Linear interpolation of two quaternions
		* @param fact Factor of interpolation. For translation from position
		* of this vector to quaternion rhs, values of factor goes from 0.0 to 1.0.
		* @param rhs Second Quaternion for interpolation
		* @note However values of fact parameter are reasonable only in interval
		* [0.0 , 1.0], you can pass also values outside of this interval and you
		* can get result (extrapolation?)
		*/
		Quaternion<T> lerp(T fact, const Quaternion<T>& rhs) const
		{
			return Quaternion<T>((1 - fact) * w + fact * rhs.w, v.lerp(fact, rhs.v));
		}

		/**
		* Provides output to standard output stream.
		*/
		friend std::ostream& operator <<(std::ostream& oss, const Quaternion<T>& q)
		{
			oss << "Re: " << q.w << " Im: " << q.v;
			return oss;
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

		/**
		* Creates quaternion from transform matrix.
		*
		* @param m Transform matrix used to compute quaternion.
		* @return Quaternion representing rotation of matrix m.
		*/
		// 2011-07-02: Davide Bacchet: changed formula to fix degenerate cases
		static Quaternion<T> fromMatrix(const Matrix4<T>& m)
		{
			Quaternion<T> q;

			T tr, s;
			tr = m(1, 1) + m(2, 2) + m(3, 3);
			if (tr >= epsilon)
			{
				s = 0.5 / (T)sqrt(tr + 1.0);
				q.w = 0.25 / s;
				q.v.x = (m(3, 2) - m(2, 3)) * s;
				q.v.y = (m(1, 3) - m(3, 1)) * s;
				q.v.z = (m(2, 1) - m(1, 2)) * s;
			}
			else
			{
				T d0 = m(1, 1);
				T d1 = m(2, 2);
				T d2 = m(3, 3);

				char bigIdx = (d0 > d1) ? ((d0 > d2) ? 0 : 2) : ((d1 > d2) ? 1 : 2);

				if (bigIdx == 0)
				{
					s = 2.0 * (T)sqrt(1.0 + m(1, 1) - m(2, 2) - m(3, 3));
					q.w = (m(3, 2) - m(2, 3)) / s;
					q.v.x = 0.25 * s;
					q.v.y = (m(1, 2) + m(2, 1)) / s;
					q.v.z = (m(1, 3) + m(3, 1)) / s;
				}
				else if (bigIdx == 1)
				{
					s = 2.0 * (T)sqrt(1.0 + m(2, 2) - m(1, 1) - m(3, 3));
					q.w = (m(1, 3) - m(3, 1)) / s;
					q.v.x = (m(1, 2) + m(2, 1)) / s;
					q.v.y = 0.25 * s;
					q.v.z = (m(2, 3) + m(3, 2)) / s;
				}
				else
				{
					s = 2.0 * (T)sqrt(1.0 + m(3, 3) - m(1, 1) - m(2, 2));
					q.w = (m(2, 1) - m(1, 2)) / s;
					q.v.x = (m(1, 3) + m(3, 1)) / s;
					q.v.y = (m(2, 3) + m(3, 2)) / s;
					q.v.z = 0.25 * s;
				}
			}

			return q;
		}

		/**
		* Creates quaternion from rotation matrix.
		*
		* @param m Rotation matrix used to compute quaternion.
		* @return Quaternion representing rotation of matrix m.
		*/

		// 2011-07-02: Davide Bacchet: changed formula to fix degenerate cases
		static Quaternion<T> fromMatrix(const Matrix3<T>& m)
		{
			Quaternion<T> q;

			T tr, s;
			tr = m(1, 1) + m(2, 2) + m(3, 3);
			if (tr >= epsilon)
			{
				s = 0.5 / (T)sqrt(tr + 1.0);
				q.w = 0.25 / s;
				q.v.x = (m(3, 2) - m(2, 3)) * s;
				q.v.y = (m(1, 3) - m(3, 1)) * s;
				q.v.z = (m(2, 1) - m(1, 2)) * s;
			}
			else
			{
				T d0 = m(1, 1);
				T d1 = m(2, 2);
				T d2 = m(3, 3);

				char bigIdx = (d0 > d1) ? ((d0 > d2) ? 0 : 2) : ((d1 > d2) ? 1 : 2);

				if (bigIdx == 0)
				{
					s = 2.0 * (T)sqrt(1.0 + m(1, 1) - m(2, 2) - m(3, 3));
					q.w = (m(3, 2) - m(2, 3)) / s;
					q.v.x = 0.25 * s;
					q.v.y = (m(1, 2) + m(2, 1)) / s;
					q.v.z = (m(1, 3) + m(3, 1)) / s;
				}
				else if (bigIdx == 1)
				{
					s = 2.0 * (T)sqrt(1.0 + m(2, 2) - m(1, 1) - m(3, 3));
					q.w = (m(1, 3) - m(3, 1)) / s;
					q.v.x = (m(1, 2) + m(2, 1)) / s;
					q.v.y = 0.25 * s;
					q.v.z = (m(2, 3) + m(3, 2)) / s;
				}
				else
				{
					s = 2.0 * (T)sqrt(1.0 + m(3, 3) - m(1, 1) - m(2, 2));
					q.w = (m(2, 1) - m(1, 2)) / s;
					q.v.x = (m(1, 3) + m(3, 1)) / s;
					q.v.y = (m(2, 3) + m(3, 2)) / s;
					q.v.z = 0.25 * s;
				}
			}

			return q;
		}

		/**
		* Computes spherical interpolation between quaternions (this, q2)
		* using coefficient of interpolation r (in [0, 1]).
		*
		* @param r The ratio of interpolation form this (r = 0) to q2 (r = 1).
		* @param q2 Second quaternion for interpolation.
		* @return Result of interpolation.
		*/
		Quaternion<T> slerp(T r, const Quaternion<T>& q2) const
		{
			Quaternion<T> ret;
			T cosTheta = w * q2.w + v.x * q2.v.x + v.y * q2.v.y + v.z * q2.v.z;
			T theta = (T)acos(cosTheta);
			if (fabs(theta) < epsilon)
			{
				ret = *this;
			}
			else
			{
				T sinTheta = (T)sqrt(1.0 - cosTheta * cosTheta);
				if (fabs(sinTheta) < epsilon)
				{
					ret.w = 0.5 * w + 0.5 * q2.w;
					ret.v = v.lerp(0.5, q2.v);
				}
				else
				{
					T rA = (T)sin((1.0 - r) * theta) / sinTheta;
					T rB = (T)sin(r * theta) / sinTheta;

					ret.w = w * rA + q2.w * rB;
					ret.v.x = v.x * rA + q2.v.x * rB;
					ret.v.y = v.y * rA + q2.v.y * rB;
					ret.v.z = v.z * rA + q2.v.z * rB;
				}
			}
			return ret;
		}

	};

	typedef Quaternion<float> Quatf;
	typedef Quaternion<double> Quatd;
};