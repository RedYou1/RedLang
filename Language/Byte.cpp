#include "Byte.h"
#include "Short.h"
#include "Integer.h"
#include "Float.h"
#include "Long.h"
#include "Double.h"

Number_Constructor_A(ByteO)
Number_Constructor_B(ByteO, int8_t)
Number_Clone(ByteO)
Number_Equals(ByteC, ByteO)
Number_Methode_Constructor(ByteC, ByteConstruct)
Number_Methode_ToString(ByteC, ByteO)
Number_Methode_LessThan(ByteC, ByteO, "Byte")
Number_Methode_LessThanEquals(ByteC, ByteO, "Byte")
Number_Methode_GreaterThan(ByteC, ByteO, "Byte")
Number_Methode_GreaterThanEquals(ByteC, ByteO, "Byte")
Number_Methode_Add(ByteC, ByteO, "Byte")
Number_Methode_Sub(ByteC, ByteO, "Byte")
Number_Methode_Mult(ByteC, ByteO, "Byte")
Number_Methode_Div(ByteC, ByteO, "Byte")
Number_Methode_Mod(ByteC, ByteO, "Byte")
Number_Methode_toThis(ByteC, toByte)
Number_Methode_toOther(ByteC, ByteO, toShort, ShortO, int16_t)
Number_Methode_toOther(ByteC, ByteO, toInteger, IntegerO, int32_t)
Number_Methode_toOther(ByteC, ByteO, toFloat, FloatO, float_t)
Number_Methode_toOther(ByteC, ByteO, toLong, LongO, int64_t)
Number_Methode_toOther(ByteC, ByteO, toDouble, DoubleO, double_t)