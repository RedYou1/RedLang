#include "Byte.h"
#include "Short.h"
#include "Integer.h"
#include "Float.h"
#include "Long.h"
#include "Double.h"
#include <cmath>

Number_Constructor_A(DoubleO)
Number_Constructor_B(DoubleO, double_t)
Number_Clone(DoubleO)
Number_Equals(DoubleC, DoubleO)
Number_Methode_Constructor(DoubleC, DoubleConstruct)
Number_Methode_ToString(DoubleC, DoubleO)
Number_Methode_LessThan(DoubleC, DoubleO, "Double")
Number_Methode_LessThanEquals(DoubleC, DoubleO, "Double")
Number_Methode_GreaterThan(DoubleC, DoubleO, "Double")
Number_Methode_GreaterThanEquals(DoubleC, DoubleO, "Double")
Number_Methode_Add(DoubleC, DoubleO, "Double", double_t)
Number_Methode_Sub(DoubleC, DoubleO, "Double", double_t)
Number_Methode_Mult(DoubleC, DoubleO, "Double", double_t)
Number_Methode_Div(DoubleC, DoubleO, "Double", double_t)
Number_Methode_fMod(DoubleC, DoubleO, "Double", double_t)
Number_Methode_toOther(DoubleC, DoubleO, toByte, ByteO, int8_t)
Number_Methode_toOther(DoubleC, DoubleO, toShort, ShortO, int16_t)
Number_Methode_toOther(DoubleC, DoubleO, toInteger, IntegerO, int32_t)
Number_Methode_toOther(DoubleC, DoubleO, toFloat, FloatO, float_t)
Number_Methode_toOther(DoubleC, DoubleO, toLong, LongO, int64_t)
Number_Methode_toThis(DoubleC, toDouble)
