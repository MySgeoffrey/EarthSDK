//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : MyS57Chart
//  @ File Name : imygeometry.h
//  @ Date : 2017/10/20
//  @ Author : sgeoffrey
//
//


#if !defined(_IMYGEOMETRY_H)
#define _IMYGEOMETRY_H

#include "mychartcommonlib/mygeoobject.h"
#include "mychartmaplib/mychartmaplib_global.h"
#include <vector>

class OGRPoint;
class OGRLineString;
namespace MyChart
{
	/// <summary>
	/// 要素几何类型
	/// </summary>
	enum GeometryType
	{
		GT_POINT,///<点

		GT_MULTIPOINT,///<多点

		GT_POLYLINE,///<折线

		GT_POLYGON,///<多边形

		GT_NONE///<无
	};

	class MYCHARTMAPLIB_EXPORT Vector3D
    {
    public:
		double x, y, z;

    public:
        inline Vector3D()
        {
        }

        inline Vector3D( const double fX, const double fY, const double fZ )
            : x( fX ), y( fY ), z( fZ )
        {
        }

        inline explicit Vector3D( const double afCoordinate[3] )
            : x( afCoordinate[0] ),
              y( afCoordinate[1] ),
              z( afCoordinate[2] )
        {
        }

        inline explicit Vector3D( const int afCoordinate[3] )
        {
            x = (double)afCoordinate[0];
            y = (double)afCoordinate[1];
            z = (double)afCoordinate[2];
        }

        inline explicit Vector3D( double* const r )
            : x( r[0] ), y( r[1] ), z( r[2] )
        {
        }

        inline explicit Vector3D( const double scaler )
            : x( scaler )
            , y( scaler )
            , z( scaler )
        {
        }


		/** Exchange the contents of this vector with another. 
		*/
		inline void swap(Vector3D& other)
		{
			std::swap(x, other.x);
			std::swap(y, other.y);
			std::swap(z, other.z);
		}

		inline double operator [] ( const size_t i ) const
        {
            //assert( i < 3 );

            return *(&x+i);
        }

		inline double& operator [] ( const size_t i )
        {
            //assert( i < 3 );

            return *(&x+i);
        }
		/// Pointer accessor for direct copying
		inline double* ptr()
		{
			return &x;
		}
		/// Pointer accessor for direct copying
		inline const double* ptr() const
		{
			return &x;
		}

        /** Assigns the value of the other vector.
            @param
                rkVector The other vector
        */
        inline Vector3D& operator = ( const Vector3D& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;
            z = rkVector.z;

            return *this;
        }

        inline Vector3D& operator = ( const double fScaler )
        {
            x = fScaler;
            y = fScaler;
            z = fScaler;

            return *this;
        }

        inline bool operator == ( const Vector3D& rkVector ) const
        {
            return ( x == rkVector.x && y == rkVector.y && z == rkVector.z );
        }

        inline bool operator != ( const Vector3D& rkVector ) const
        {
            return ( x != rkVector.x || y != rkVector.y || z != rkVector.z );
        }

        // arithmetic operations
        inline Vector3D operator + ( const Vector3D& rkVector ) const
        {
            return Vector3D(
                x + rkVector.x,
                y + rkVector.y,
                z + rkVector.z);
        }

        inline Vector3D operator - ( const Vector3D& rkVector ) const
        {
            return Vector3D(
                x - rkVector.x,
                y - rkVector.y,
                z - rkVector.z);
        }

        inline Vector3D operator * ( const double fScalar ) const
        {
            return Vector3D(
                x * fScalar,
                y * fScalar,
                z * fScalar);
        }

        inline Vector3D operator * ( const Vector3D& rhs) const
        {
            return Vector3D(
                x * rhs.x,
                y * rhs.y,
                z * rhs.z);
        }

        inline Vector3D operator / ( const double fScalar ) const
        {
           // assert( fScalar != 0.0 );

            double fInv = 1.0f / fScalar;

            return Vector3D(
                x * fInv,
                y * fInv,
                z * fInv);
        }

        inline Vector3D operator / ( const Vector3D& rhs) const
        {
            return Vector3D(
                x / rhs.x,
                y / rhs.y,
                z / rhs.z);
        }

        inline const Vector3D& operator + () const
        {
            return *this;
        }

        inline Vector3D operator - () const
        {
            return Vector3D(-x, -y, -z);
        }

        // overloaded operators to help Vector3D
        inline friend Vector3D operator * ( const double fScalar, const Vector3D& rkVector )
        {
            return Vector3D(
                fScalar * rkVector.x,
                fScalar * rkVector.y,
                fScalar * rkVector.z);
        }

        inline friend Vector3D operator / ( const double fScalar, const Vector3D& rkVector )
        {
            return Vector3D(
                fScalar / rkVector.x,
                fScalar / rkVector.y,
                fScalar / rkVector.z);
        }

        inline friend Vector3D operator + (const Vector3D& lhs, const double rhs)
        {
            return Vector3D(
                lhs.x + rhs,
                lhs.y + rhs,
                lhs.z + rhs);
        }

        inline friend Vector3D operator + (const double lhs, const Vector3D& rhs)
        {
            return Vector3D(
                lhs + rhs.x,
                lhs + rhs.y,
                lhs + rhs.z);
        }

        inline friend Vector3D operator - (const Vector3D& lhs, const double rhs)
        {
            return Vector3D(
                lhs.x - rhs,
                lhs.y - rhs,
                lhs.z - rhs);
        }

        inline friend Vector3D operator - (const double lhs, const Vector3D& rhs)
        {
            return Vector3D(
                lhs - rhs.x,
                lhs - rhs.y,
                lhs - rhs.z);
        }

        // arithmetic updates
        inline Vector3D& operator += ( const Vector3D& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;
            z += rkVector.z;

            return *this;
        }

        inline Vector3D& operator += ( const double fScalar )
        {
            x += fScalar;
            y += fScalar;
            z += fScalar;
            return *this;
        }

        inline Vector3D& operator -= ( const Vector3D& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;
            z -= rkVector.z;

            return *this;
        }

        inline Vector3D& operator -= ( const double fScalar )
        {
            x -= fScalar;
            y -= fScalar;
            z -= fScalar;
            return *this;
        }

        inline Vector3D& operator *= ( const double fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            z *= fScalar;
            return *this;
        }

        inline Vector3D& operator *= ( const Vector3D& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;
            z *= rkVector.z;

            return *this;
        }

        inline Vector3D& operator /= ( const double fScalar )
        {
            //assert( fScalar != 0.0 );

            double fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;

            return *this;
        }

        inline Vector3D& operator /= ( const Vector3D& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;
            z /= rkVector.z;

            return *this;
        }


        /** Returns the length (magnitude) of the vector.
            @warning
                This operation requires a square root and is expensive in
                terms of CPU operations. If you don't need to know the exact
                length (e.g. for just comparing lengths) use squaredLength()
                instead.
        */
        inline double length () const
        {
            return std::sqrt( x * x + y * y + z * z );
        }

        /** Returns the square of the length(magnitude) of the vector.
            @remarks
                This  method is for efficiency - calculating the actual
                length of a vector requires a square root, which is expensive
                in terms of the operations required. This method returns the
                square of the length of the vector, i.e. the same as the
                length but before the square root is taken. Use this if you
                want to find the longest / shortest vector without incurring
                the square root.
        */
        inline double squaredLength () const
        {
            return x * x + y * y + z * z;
        }

        /** Returns the distance to another vector.
            @warning
                This operation requires a square root and is expensive in
                terms of CPU operations. If you don't need to know the exact
                distance (e.g. for just comparing distances) use squaredDistance()
                instead.
        */
        inline double distance(const Vector3D& rhs) const
        {
            return (*this - rhs).length();
        }

        /** Returns the square of the distance to another vector.
            @remarks
                This method is for efficiency - calculating the actual
                distance to another vector requires a square root, which is
                expensive in terms of the operations required. This method
                returns the square of the distance to another vector, i.e.
                the same as the distance but before the square root is taken.
                Use this if you want to find the longest / shortest distance
                without incurring the square root.
        */
        inline double squaredDistance(const Vector3D& rhs) const
        {
            return (*this - rhs).squaredLength();
        }

        /** Calculates the dot (scalar) product of this vector with another.
            @remarks
                The dot product can be used to calculate the angle between 2
                vectors. If both are unit vectors, the dot product is the
                cosine of the angle; otherwise the dot product must be
                divided by the product of the lengths of both vectors to get
                the cosine of the angle. This result can further be used to
                calculate the distance of a point from a plane.
            @param
                vec Vector with which to calculate the dot product (together
                with this one).
            @return
                A float representing the dot product value.
        */
        inline double dotProduct(const Vector3D& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }

        /** Calculates the absolute dot (scalar) product of this vector with another.
            @remarks
                This function work similar dotProduct, except it use absolute value
                of each component of the vector to computing.
            @param
                vec Vector with which to calculate the absolute dot product (together
                with this one).
            @return
                A double representing the absolute dot product value.
        */
        inline double absDotProduct(const Vector3D& vec) const
        {
            return std::abs(x * vec.x) + std::abs(y * vec.y) + std::abs(z * vec.z);
        }

        /** Normalises the vector.
            @remarks
                This method normalises the vector such that it's
                length / magnitude is 1. The result is called a unit vector.
            @note
                This function will not crash for zero-sized vectors, but there
                will be no changes made to their components.
            @return The previous length of the vector.
        */
        inline double normalise()
        {
            double fLength = std::sqrt( x * x + y * y + z * z );

            // Will also work for zero-sized vectors, but will change nothing
			// We're not using epsilons because we don't need to.
            // Read http://www.ogre3d.org/forums/viewtopic.php?f=4&t=61259
            if ( fLength > double(0.0f) )
            {
                double fInvLength = 1.0f / fLength;
                x *= fInvLength;
                y *= fInvLength;
                z *= fInvLength;
            }

            return fLength;
        }

        /** Calculates the cross-product of 2 vectors, i.e. the vector that
            lies perpendicular to them both.
            @remarks
                The cross-product is normally used to calculate the normal
                vector of a plane, by calculating the cross-product of 2
                non-equivalent vectors which lie on the plane (e.g. 2 edges
                of a triangle).
            @param
                vec Vector which, together with this one, will be used to
                calculate the cross-product.
            @return
                A vector which is the result of the cross-product. This
                vector will <b>NOT</b> be normalised, to maximise efficiency
                - call Vector3D::normalise on the result if you wish this to
                be done. As for which side the resultant vector will be on, the
                returned vector will be on the side from which the arc from 'this'
                to rkVector is anticlockwise, e.g. UNIT_Y.crossProduct(UNIT_Z)
                = UNIT_X, whilst UNIT_Z.crossProduct(UNIT_Y) = -UNIT_X.
				This is because OGRE uses a right-handed coordinate system.
            @par
                For a clearer explanation, look a the left and the bottom edges
                of your monitor's screen. Assume that the first vector is the
                left edge and the second vector is the bottom edge, both of
                them starting from the lower-left corner of the screen. The
                resulting vector is going to be perpendicular to both of them
                and will go <i>inside</i> the screen, towards the cathode tube
                (assuming you're using a CRT monitor, of course).
        */
        inline Vector3D crossProduct( const Vector3D& rkVector ) const
        {
            return Vector3D(
                y * rkVector.z - z * rkVector.y,
                z * rkVector.x - x * rkVector.z,
                x * rkVector.y - y * rkVector.x);
        }

        /** Returns a vector at a point half way between this and the passed
            in vector.
        */
        inline Vector3D midPoint( const Vector3D& vec ) const
        {
            return Vector3D(
                ( x + vec.x ) * 0.5f,
                ( y + vec.y ) * 0.5f,
                ( z + vec.z ) * 0.5f );
        }

        /** Returns true if the vector's scalar components are all greater
            that the ones of the vector it is compared against.
        */
        inline bool operator < ( const Vector3D& rhs ) const
        {
            if( x < rhs.x && y < rhs.y && z < rhs.z )
                return true;
            return false;
        }

        /** Returns true if the vector's scalar components are all smaller
            that the ones of the vector it is compared against.
        */
        inline bool operator > ( const Vector3D& rhs ) const
        {
            if( x > rhs.x && y > rhs.y && z > rhs.z )
                return true;
            return false;
        }

        /** Sets this vector's components to the minimum of its own and the
            ones of the passed in vector.
            @remarks
                'Minimum' in this case means the combination of the lowest
                value of x, y and z from both vectors. Lowest is taken just
                numerically, not magnitude, so -1 < 0.
        */
        inline void makeFloor( const Vector3D& cmp )
        {
            if( cmp.x < x ) x = cmp.x;
            if( cmp.y < y ) y = cmp.y;
            if( cmp.z < z ) z = cmp.z;
        }

        /** Sets this vector's components to the maximum of its own and the
            ones of the passed in vector.
            @remarks
                'Maximum' in this case means the combination of the highest
                value of x, y and z from both vectors. Highest is taken just
                numerically, not magnitude, so 1 > -3.
        */
        inline void makeCeil( const Vector3D& cmp )
        {
            if( cmp.x > x ) x = cmp.x;
            if( cmp.y > y ) y = cmp.y;
            if( cmp.z > z ) z = cmp.z;
        }

        /** Generates a vector perpendicular to this vector (eg an 'up' vector).
            @remarks
                This method will return a vector which is perpendicular to this
                vector. There are an infinite number of possibilities but this
                method will guarantee to generate one of them. If you need more
                control you should use the Quaternion class.
        */
        inline Vector3D perpendicular(void) const
        {
            static const double fSquareZero = (double)(1e-06 * 1e-06);

            Vector3D perp = this->crossProduct( Vector3D::UNIT_X );

            // Check length
            if( perp.squaredLength() < fSquareZero )
            {
                /* This vector is the Y axis multiplied by a scalar, so we have
                   to use another axis.
                */
                perp = this->crossProduct( Vector3D::UNIT_Y );
            }
			perp.normalise();

            return perp;
        }
        /** Generates a new random vector which deviates from this vector by a
            given angle in a random direction.
            @remarks
                This method assumes that the random number generator has already
                been seeded appropriately.
            @param
                angle The angle at which to deviate
            @param
                up Any vector perpendicular to this one (which could generated
                by cross-product of this vector and any other non-colinear
                vector). If you choose not to provide this the function will
                derive one on it's own, however if you provide one yourself the
                function will be faster (this allows you to reuse up vectors if
                you call this method more than once)
            @return
                A random vector which deviates from this vector by angle. This
                vector will not be normalised, normalise it if you wish
                afterwards.
        */


		/** Gets the angle between 2 vectors.
		@remarks
			Vectors do not have to be unit-length but must represent directions.
		*/
		inline double angleBetween(const Vector3D& dest) const
		{
			double lenProduct = length() * dest.length();

			// Divide by zero check
			if(lenProduct < 1e-6f)
				lenProduct = 1e-6f;

			double f = dotProduct(dest) / lenProduct;

			//f = std::clamp(f, (double)-1.0, (double)1.0);
			return std::acos(f);

		}


        /** Returns true if this vector is zero length. */
        inline bool isZeroLength(void) const
        {
            double sqlen = (x * x) + (y * y) + (z * z);
            return (sqlen < (1e-06 * 1e-06));

        }

        /** As normalise, except that this vector is unaffected and the
            normalised vector is returned as a copy. */
        inline Vector3D normalisedCopy(void) const
        {
            Vector3D ret = *this;
            ret.normalise();
            return ret;
        }

        /** Calculates a reflection vector to the plane with the given normal .
        @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
        */
        inline Vector3D reflect(const Vector3D& normal) const
        {
            return Vector3D( *this - ( 2 * this->dotProduct(normal) * normal ) );
        }


		/** Returns whether this vector is within a positional tolerance
			of another vector, also take scale of the vectors into account.
		@param rhs The vector to compare with
		@param tolerance The amount (related to the scale of vectors) that distance
            of the vector may vary by and still be considered close
		*/
		inline bool positionCloses(const Vector3D& rhs, double tolerance = 1e-03f) const
		{
			return squaredDistance(rhs) <=
                (squaredLength() + rhs.squaredLength()) * tolerance;
		}


		// special points
        static const Vector3D ZERO;
        static const Vector3D UNIT_X;
        static const Vector3D UNIT_Y;
        static const Vector3D UNIT_Z;
        static const Vector3D NEGATIVE_UNIT_X;
        static const Vector3D NEGATIVE_UNIT_Y;
        static const Vector3D NEGATIVE_UNIT_Z;
        static const Vector3D UNIT_SCALE;

    };

	/// <summary>
	/// 空间坐标Z（默认Z为0）
	/// </summary>
	class MYCHARTMAPLIB_EXPORT IVector3
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		IVector3()
		{
			X = 0;
			Y = 0;
			Z = 0;
		}

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="x">x坐标值</param>
		/// <param name="y">y坐标值</param>
		/// <param name="z">z坐标值</param>
		IVector3(const double& x,const double& y,const double& z = 0)
			: X(x),Y(y),Z(z)
		{

		}

		Vector3D toVector3D()
		{
			return Vector3D(this->X,this->Y,this->Z);
		}

		/// <summary>
		/// 重载==
		/// </summary>
		/// <param name="other">当前坐标与other是否相等</param>
		/// <returns>是否相等</returns>
		bool operator==(IVector3& other)
		{
			if (this->X == other.X
				&& this->Y == other.Y
				&& this->Z == other.Z)
				return true;
			return false;
		}

		/// <summary>
		/// 重载-
		/// </summary>
		/// <param name="other">当前坐标与other相减</param>
		/// <returns>相减后的结果</returns>
		IVector3 operator-(IVector3& other)
		{
			return IVector3(
				this->X - other.X,
				this->Y - other.Y,
				this->Z - other.Z);
		}

		IVector3 operator+(IVector3& other)
		{
			return IVector3(
				this->X + other.X,
				this->Y + other.Y,
				this->Z + other.Z);
		}

		IVector3 operator*(double& scale)
		{
			return IVector3(
				this->X * scale,
				this->Y * scale,
				this->Z * scale);
		}

		/// <summary>
		/// 向量的长度
		/// </summary>
		/// <returns>长度值</returns>
		double length()
		{
			double sqrtValue = std::sqrt(X * X + Y * Y + Z * Z);
			return sqrtValue;
		}
		/// <summary>
		/// 向量归一化
		/// </summary>
		/// <returns>归一化之前的长度</returns>
		double normalise()
		{
			double sqrtValue = std::sqrt(X * X + Y * Y + Z * Z);
			X /= sqrtValue;
			Y /= sqrtValue;
			Z /= sqrtValue;
			return sqrtValue;
		}
		/// <summary>
		/// 向量V1点成v2
		/// </summary>
		/// <param name="v1">第一个向量</param>
		/// <param name="v2">第二个向量</param>
		/// <returns>点成结果</returns>
		static double dot(IVector3& v1,IVector3& v2)
		{
			return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z;
		}

	public:
		double X;
		double Y;
		double Z;
	};

	

	/// <summary>
	/// 几何要素基础
	/// </summary>
	class MYCHARTMAPLIB_EXPORT IMyGeometry
		: public IMyGeoObject
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		IMyGeometry();

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">名称</param>
		IMyGeometry(const QString& name);

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IMyGeometry();

		/// <summary>
		/// 获取几何类型
		/// </summary>
		/// <returns>几何类型</returns>
		virtual GeometryType& getType();

		/// <summary>
		/// 获取几何要素的各部分数目
		/// </summary>
		/// <returns>各部分数目</returns>
		virtual int numOfParts();

		/// <summary>
		/// 获取几何要素的点个数
		/// </summary>
		/// <returns>点个数</returns>
		virtual int numOfPoints();

	protected:
		GeometryType mGeometryType;
	};

	/// <summary>
	/// 获取几何点要素
	/// </summary>
	class MYCHARTMAPLIB_EXPORT IMyPoint
		: public IMyGeometry
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		IMyPoint();

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">名称</param>
		IMyPoint(const QString& name);

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IMyPoint();

		/// <summary>
		/// 获取几何要素的各部分数目
		/// </summary>
		/// <returns>各部分数目</returns>
		virtual int numOfParts();

		/// <summary>
		/// 获取几何要素的点个数
		/// </summary>
		/// <returns>点个数</returns>
		virtual int numOfPoints();
	public:
		IVector3 Position;
	};

	/// <summary>
	/// 几何多点要素类
	/// </summary>
	class MYCHARTMAPLIB_EXPORT IMyMultiPoint
		: public IMyGeometry
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		IMyMultiPoint();

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">名称</param>
		IMyMultiPoint(const QString& name);

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IMyMultiPoint();

		/// <summary>
		/// 获取几何要素的各部分数目
		/// </summary>
		/// <returns>各部分数目</returns>
		virtual int numOfParts();

		/// <summary>
		/// 获取几何要素的点个数
		/// </summary>
		/// <returns>点个数</returns>
		virtual int numOfPoints();
	public:
		int NumPoints;///<点个数
		std::vector<IVector3> Points;///<点集
	};
	
	/// <summary>
	/// 几何折线类
	/// </summary>
	class MYCHARTMAPLIB_EXPORT IMyPolyline
		: public IMyGeometry
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		IMyPolyline();

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">名称</param>
		IMyPolyline(const QString& name);

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IMyPolyline();

		/// <summary>
		/// 获取几何要素的各部分数目
		/// </summary>
		/// <returns>各部分数目</returns>
		virtual int numOfParts();

		/// <summary>
		/// 获取几何要素的点个数
		/// </summary>
		/// <returns>点个数</returns>
		virtual int numOfPoints();

		virtual bool sortByLatitude(const bool& acend);

		virtual bool sortByLongitude(const bool& acend);

		virtual IMyPolyline* buffer(const double& bufferDistance,const int& type);

		virtual bool contain(const IVector3& lineStartPoint,const IVector3& lineEndPoint);

		virtual IMyPolyline* intersect(const IVector3& lineStartPoint,const IVector3& lineEndPoint);

		virtual bool intersectWith(const IVector3& lineStartPoint,const IVector3& lineEndPoint);
	public:
		int NumParts;///<各部分数目
		int NumPoints;///<点数目
		std::vector<int> Parts;///<各部分数据
		std::vector<IVector3> Points;///<点集
	};
	
	/// <summary>
	/// 多边形
	/// </summary>
	class MYCHARTMAPLIB_EXPORT IMyPolygon 
		: public IMyPolyline
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		IMyPolygon();

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">名称</param>
		IMyPolygon(const QString& name);

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IMyPolygon();

	};

}
#endif  //_IMYGEOMETRY_H
