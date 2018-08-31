/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2014 Markus Schöngart
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef ROCKETCORETRANSFORMPRIMITIVE_H
#define ROCKETCORETRANSFORMPRIMITIVE_H

#include "Header.h"
#include "Types.h"
#include "Property.h"
#include <variant>
#include <array>

namespace Rocket {
namespace Core {
namespace Transforms {
	

struct NumericValue
{
	/// Non-initializing constructor.
	NumericValue() noexcept;
	/// Construct from a float and a Unit.
	NumericValue(float number, Property::Unit unit) noexcept;

	/// Resolve a numeric property value for an element.
	float Resolve(Element& e, float base) const noexcept;
	/// Resolve a numeric property value with the element's width as relative base value.
	float ResolveWidth(Element& e) const noexcept;
	/// Resolve a numeric property value with the element's height as relative base value.
	float ResolveHeight(Element& e) const noexcept;
	/// Resolve a numeric property value with the element's depth as relative base value.
	float ResolveDepth(Element& e) const noexcept;
	/// Returns the numeric value converted to base_unit, or 'number' if no relationship defined.
	/// Defined for: {Number, Deg, %} -> Rad
	float ResolveAbsoluteUnit(Property::Unit base_unit) const noexcept;

	float number;
	Property::Unit unit;
};


template< size_t N >
struct ResolvedPrimitive
{
	ResolvedPrimitive(const float* values) noexcept
	{
		for (size_t i = 0; i < N; ++i)
			this->values[i] = values[i];
	}
	ResolvedPrimitive(const NumericValue* values) noexcept
	{
		for (size_t i = 0; i < N; ++i) 
			this->values[i] = values[i].number;
	}
	ResolvedPrimitive(const NumericValue* values, std::array<Property::Unit, N> base_units) noexcept
	{
		for (size_t i = 0; i < N; ++i)
			this->values[i] = values[i].ResolveAbsoluteUnit(base_units[i]);
	}
	ResolvedPrimitive(std::array<NumericValue, N> values, std::array<Property::Unit, N> base_units) noexcept
	{
		for (size_t i = 0; i < N; ++i)
			this->values[i] = values[i].ResolveAbsoluteUnit(base_units[i]);
	}
	ResolvedPrimitive(std::array<float, N> values) noexcept : values(values) { }
	
	std::array<float, N> values;
};

template< size_t N >
struct UnresolvedPrimitive
{
	UnresolvedPrimitive(const NumericValue* values) noexcept
	{
		for (size_t i = 0; i < N; ++i)
			this->values[i] = values[i];
	}
	UnresolvedPrimitive(std::array<NumericValue, N> values) noexcept : values(values) { }

	std::array<NumericValue, N> values;
};





struct Matrix2D : public ResolvedPrimitive< 6 >
{
	Matrix2D(const NumericValue* values) noexcept : ResolvedPrimitive(values) { }
};

struct Matrix3D : public ResolvedPrimitive< 16 >
{
	Matrix3D(const Matrix4f& matrix) noexcept : ResolvedPrimitive(matrix.data()) { }
	Matrix3D(const NumericValue* values) noexcept : ResolvedPrimitive(values) { }
};

struct TranslateX : public UnresolvedPrimitive< 1 >
{
	TranslateX(const NumericValue* values) noexcept : UnresolvedPrimitive(values) { }
	TranslateX(float x, Property::Unit unit = Property::PX) noexcept : UnresolvedPrimitive({ NumericValue{x, unit} }) { }
};

struct TranslateY : public UnresolvedPrimitive< 1 >
{
	TranslateY(const NumericValue* values) noexcept : UnresolvedPrimitive(values) { }
	TranslateY(float y, Property::Unit unit = Property::PX) noexcept : UnresolvedPrimitive({ NumericValue(y, unit) }) { }
};

struct TranslateZ : public UnresolvedPrimitive< 1 >
{
	TranslateZ(const NumericValue* values) noexcept : UnresolvedPrimitive(values) { }
	TranslateZ(float z, Property::Unit unit = Property::PX) noexcept : UnresolvedPrimitive({ NumericValue(z, unit) }) { }
};

struct Translate2D : public UnresolvedPrimitive< 2 >
{
	Translate2D(const NumericValue* values) noexcept : UnresolvedPrimitive(values) { }
	Translate2D(float x, float y, Property::Unit units = Property::PX) noexcept : UnresolvedPrimitive({ NumericValue(x, units), NumericValue(y, units) }) { }
};

struct Translate3D : public UnresolvedPrimitive< 3 >
{
	Translate3D(const NumericValue* values) noexcept : UnresolvedPrimitive(values) { }
	Translate3D(NumericValue x, NumericValue y, NumericValue z) noexcept : UnresolvedPrimitive({ x, y, z }) { }
	Translate3D(float x, float y, float z, Property::Unit units = Property::PX) noexcept : UnresolvedPrimitive({ NumericValue(x, units), NumericValue(y, units), NumericValue(z, units) }) { }
};

struct ScaleX : public ResolvedPrimitive< 1 >
{
	ScaleX(const NumericValue* values) noexcept : ResolvedPrimitive(values) { }
	ScaleX(float value) noexcept : ResolvedPrimitive({ value }) { }
};

struct ScaleY : public ResolvedPrimitive< 1 >
{
	ScaleY(const NumericValue* values) noexcept : ResolvedPrimitive(values) { }
	ScaleY(float value) noexcept : ResolvedPrimitive({ value }) { }
};

struct ScaleZ : public ResolvedPrimitive< 1 >
{
	ScaleZ(const NumericValue* values) noexcept : ResolvedPrimitive(values) { }
	ScaleZ(float value) noexcept : ResolvedPrimitive({ value }) { }
};

struct Scale2D : public ResolvedPrimitive< 2 >
{
	Scale2D(const NumericValue* values) noexcept : ResolvedPrimitive(values) { }
	Scale2D(float xy) noexcept : ResolvedPrimitive({ xy, xy }) { }
	Scale2D(float x, float y) noexcept : ResolvedPrimitive({ x, y }) { }
};

struct Scale3D : public ResolvedPrimitive< 3 >
{
	Scale3D(const NumericValue* values) noexcept : ResolvedPrimitive(values) { }
	Scale3D(float xyz) noexcept : ResolvedPrimitive({ xyz, xyz, xyz }) { }
	Scale3D(float x, float y, float z) noexcept : ResolvedPrimitive({ x, y, z }) { }
};

struct RotateX : public ResolvedPrimitive< 1 >
{
	RotateX(const NumericValue* values) noexcept : ResolvedPrimitive(values, { Property::RAD }) { }
	RotateX(float angle, Property::Unit unit = Property::DEG) noexcept : ResolvedPrimitive({ NumericValue{ angle, unit } }, { Property::RAD }) { }
};

struct RotateY : public ResolvedPrimitive< 1 >
{
	RotateY(const NumericValue* values) noexcept : ResolvedPrimitive(values, { Property::RAD }) {}
	RotateY(float angle, Property::Unit unit = Property::DEG) noexcept : ResolvedPrimitive({ NumericValue{ angle, unit } }, { Property::RAD }) { }
};

struct RotateZ : public ResolvedPrimitive< 1 >
{
	RotateZ(const NumericValue* values) noexcept : ResolvedPrimitive(values, { Property::RAD }) { }
	RotateZ(float angle, Property::Unit unit = Property::DEG) noexcept : ResolvedPrimitive({ NumericValue{ angle, unit } }, { Property::RAD }) { }
};

struct Rotate2D : public ResolvedPrimitive< 1 >
{
	Rotate2D(const NumericValue* values) noexcept : ResolvedPrimitive(values, { Property::RAD }) { }
	Rotate2D(float angle, Property::Unit unit = Property::DEG) noexcept : ResolvedPrimitive({ NumericValue{ angle, unit } }, { Property::RAD }) { }
};

struct Rotate3D : public ResolvedPrimitive< 4 >
{
	Rotate3D(const NumericValue* values) noexcept : ResolvedPrimitive(values, { Property::NUMBER, Property::NUMBER, Property::NUMBER, Property::RAD }) { }
	Rotate3D(float x, float y, float z, float angle, Property::Unit angle_unit = Property::DEG) noexcept
		: ResolvedPrimitive({ NumericValue{x, Property::NUMBER}, NumericValue{y, Property::NUMBER}, NumericValue{z, Property::NUMBER}, NumericValue{angle, angle_unit} },
			{ Property::NUMBER, Property::NUMBER, Property::NUMBER, Property::RAD }) { }
};

struct SkewX : public ResolvedPrimitive< 1 >
{
	SkewX(const NumericValue* values) noexcept : ResolvedPrimitive(values, { Property::RAD }) { }
	SkewX(float angle, Property::Unit unit = Property::DEG) noexcept : ResolvedPrimitive({ NumericValue{ angle, unit } }, { Property::RAD }) { }
};

struct SkewY : public ResolvedPrimitive< 1 >
{
	SkewY(const NumericValue* values) noexcept : ResolvedPrimitive(values, { Property::RAD }) { }
	SkewY(float angle, Property::Unit unit = Property::DEG) noexcept : ResolvedPrimitive({ NumericValue{ angle, unit } }, { Property::RAD }) { }
};

struct Skew2D : public ResolvedPrimitive< 2 >
{
	Skew2D(const NumericValue* values) noexcept : ResolvedPrimitive(values, { Property::RAD, Property::RAD }) { }
	Skew2D(float x, float y, Property::Unit unit = Property::DEG) noexcept : ResolvedPrimitive({NumericValue{ x, unit }, { NumericValue{ y, unit }}}, {Property::RAD, Property::RAD}) { }
};

struct Perspective : public UnresolvedPrimitive< 1 >
{
	Perspective(const NumericValue* values) noexcept : UnresolvedPrimitive(values) { }
};

struct DecomposedMatrix4
{
	Vector4f perspective;
	Vector4f quaternion;
	Vector3f translation;
	Vector3f scale;
	Vector3f skew;

	bool Decompose(const Matrix4f& m);
};


using PrimitiveVariant = std::variant<
	Matrix2D, Matrix3D,
	TranslateX, TranslateY, TranslateZ, Translate2D, Translate3D,
	ScaleX, ScaleY, ScaleZ, Scale2D, Scale3D,
	RotateX, RotateY, RotateZ, Rotate2D, Rotate3D,
	SkewX, SkewY, Skew2D,
	Perspective, DecomposedMatrix4>;


/**
	The Primitive struct is the base struct of geometric transforms such as rotations, scalings and translations.
	Instances of this struct are added to a Rocket::Core::Transform instance
	by the Rocket::Core::PropertyParserTransform, which is responsible for
	parsing the `transform' property.

	@author Markus Schöngart
	@see Rocket::Core::Transform
	@see Rocket::Core::PropertyParserTransform
 */
struct Primitive
{
	PrimitiveVariant primitive;

	template<typename PrimitiveType>
	Primitive(PrimitiveType primitive) : primitive(primitive) {}

	void SetIdentity() noexcept;

	bool ResolveTransform(Matrix4f& m, Element& e) const noexcept;
	bool ResolvePerspective(float &p, Element& e) const noexcept;
	
	// Prepares this primitive for interpolation. This must be done before calling InterpolateWith().
	// Promote units to basic types which can be interpolated, that is, convert 'length -> pixel' for unresolved primitives.
	// Returns false if the owning transform must to be converted to a DecomposedMatrix4 primitive.
	bool PrepareForInterpolation(Element& e) noexcept;

	// If primitives do not match, try to convert them to a common generic type, e.g. TranslateX -> Translate3D.
	// Returns true if they are already the same type or were converted to a common generic type.
	static bool TryConvertToMatchingGenericType(Primitive& p0, Primitive& p1) noexcept;

	// Interpolate this primitive with another primitive, weighted by alpha [0, 1].
	// Primitives must be of same type and PrepareForInterpolation() must previously have been called on both.
	bool InterpolateWith(const Primitive& other, float alpha) noexcept;

};


}
}
}

#endif