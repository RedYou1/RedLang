#include "Byte.h"
#include "Short.h"
#include "Integer.h"
#include "Float.h"
#include "Long.h"
#include "Double.h"
#include <cmath>

Number_Constructor_A(FloatO)
Number_Constructor_B(FloatO, float_t)
Number_Clone(FloatO)
Number_Equals(FloatC, FloatO)
Number_Methode_Constructor(FloatC, FloatConstruct)
Number_Methode_ToString(FloatC, FloatO)
Number_Methode_LessThan(FloatC, FloatO, "Float")
Number_Methode_LessThanEquals(FloatC, FloatO, "Float")
Number_Methode_GreaterThan(FloatC, FloatO, "Float")
Number_Methode_GreaterThanEquals(FloatC, FloatO, "Float")
Number_Methode_Add(FloatC, FloatO, "Float", float_t)
Number_Methode_Sub(FloatC, FloatO, "Float", float_t)
Number_Methode_Mult(FloatC, FloatO, "Float", float_t)
Number_Methode_Div(FloatC, FloatO, "Float", float_t)
Number_Methode_fMod(FloatC, FloatO, "Float", float_t)
Number_Methode_toOther(FloatC, FloatO, toByte, ByteO, int8_t)
Number_Methode_toOther(FloatC, FloatO, toShort, ShortO, int16_t)
Number_Methode_toOther(FloatC, FloatO, toInteger, IntegerO, int32_t)
Number_Methode_toThis(FloatC, toFloat)
Number_Methode_toOther(FloatC, FloatO, toLong, LongO, int64_t)
Number_Methode_toOther(FloatC, FloatO, toDouble, DoubleO, double_t)