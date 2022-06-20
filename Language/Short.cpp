#include "Byte.h"
#include "Short.h"
#include "Integer.h"
#include "Float.h"
#include "Long.h"
#include "Double.h"

Number_Constructor_A(ShortO)
Number_Constructor_B(ShortO, int16_t)
Number_Clone(ShortO)
Number_Equals(ShortC, ShortO)
Number_Methode_Constructor(ShortC, ShortConstruct)
Number_Methode_ToString(ShortC, ShortO)
Number_Methode_LessThan(ShortC, ShortO, "Short")
Number_Methode_LessThanEquals(ShortC, ShortO, "Short")
Number_Methode_GreaterThan(ShortC, ShortO, "Short")
Number_Methode_GreaterThanEquals(ShortC, ShortO, "Short")
Number_Methode_Add(ShortC, ShortO, "Short")
Number_Methode_Sub(ShortC, ShortO, "Short")
Number_Methode_Mult(ShortC, ShortO, "Short")
Number_Methode_Div(ShortC, ShortO, "Short")
Number_Methode_Mod(ShortC, ShortO, "Short")
Number_Methode_toOther(ShortC, ShortO, toByte, ByteO, int8_t)
Number_Methode_toThis(ShortC, toShort)
Number_Methode_toOther(ShortC, ShortO, toInteger, IntegerO, int32_t)
Number_Methode_toOther(ShortC, ShortO, toFloat, FloatO, float_t)
Number_Methode_toOther(ShortC, ShortO, toLong, LongO, int64_t)
Number_Methode_toOther(ShortC, ShortO, toDouble, DoubleO, double_t)