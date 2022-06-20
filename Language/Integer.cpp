#include "Byte.h"
#include "Short.h"
#include "Integer.h"
#include "Float.h"
#include "Long.h"
#include "Double.h"

Number_Constructor_A(IntegerO)
Number_Constructor_B(IntegerO, int32_t)
Number_Clone(IntegerO)
Number_Equals(IntegerC, IntegerO)
Number_Methode_Constructor(IntegerC, IntegerConstruct)
Number_Methode_ToString(IntegerC, IntegerO)
Number_Methode_LessThan(IntegerC, IntegerO, "Integer")
Number_Methode_LessThanEquals(IntegerC, IntegerO, "Integer")
Number_Methode_GreaterThan(IntegerC, IntegerO, "Integer")
Number_Methode_GreaterThanEquals(IntegerC, IntegerO, "Integer")
Number_Methode_Add(IntegerC, IntegerO, "Integer")
Number_Methode_Sub(IntegerC, IntegerO, "Integer")
Number_Methode_Mult(IntegerC, IntegerO, "Integer")
Number_Methode_Div(IntegerC, IntegerO, "Integer")
Number_Methode_Mod(IntegerC, IntegerO, "Integer")
Number_Methode_toOther(IntegerC, IntegerO, toByte, ByteO, int8_t)
Number_Methode_toOther(IntegerC, IntegerO, toShort, ShortO, int16_t)
Number_Methode_toThis(IntegerC, toInteger)
Number_Methode_toOther(IntegerC, IntegerO, toFloat, FloatO, float_t)
Number_Methode_toOther(IntegerC, IntegerO, toLong, LongO, int64_t)
Number_Methode_toOther(IntegerC, IntegerO, toDouble, DoubleO, double_t)