#include "Byte.h"
#include "Short.h"
#include "Integer.h"
#include "Float.h"
#include "Long.h"
#include "Double.h"

Number_Constructor_A(LongO)
Number_Constructor_B(LongO, int64_t)
Number_Clone(LongO)
Number_Equals(LongC, LongO)
Number_Methode_Constructor(LongC, LongConstruct)
Number_Methode_ToString(LongC, LongO)
Number_Methode_LessThan(LongC, LongO, "Long")
Number_Methode_LessThanEquals(LongC, LongO, "Long")
Number_Methode_GreaterThan(LongC, LongO, "Long")
Number_Methode_GreaterThanEquals(LongC, LongO, "Long")
Number_Methode_Add(LongC, LongO, "Long", int64_t)
Number_Methode_Sub(LongC, LongO, "Long", int64_t)
Number_Methode_Mult(LongC, LongO, "Long", int64_t)
Number_Methode_Div(LongC, LongO, "Long", int64_t)
Number_Methode_Mod(LongC, LongO, "Long", int64_t)
Number_Methode_toOther(LongC, LongO, toByte, ByteO, int8_t)
Number_Methode_toOther(LongC, LongO, toShort, ShortO, int16_t)
Number_Methode_toOther(LongC, LongO, toInteger, IntegerO, int32_t)
Number_Methode_toOther(LongC, LongO, toFloat, FloatO, float_t)
Number_Methode_toThis(LongC, toLong)
Number_Methode_toOther(LongC, LongO, toDouble, DoubleO, double_t)