#ifndef TypeListH
#define TypeListH

#include <typeinfo>
#include <string>
#include "TypeTraits.h"

template<typename T, typename U>
struct TypeList
{
	typedef T Head;
	typedef U Tail;
};

#define TypeList01(T01) TypeList<T01, NullType>
#define TypeList02(T01, T02) \
	TypeList<T01, TypeList01(T02) >
#define TypeList03(T01, T02, T03) \
	TypeList<T01, TypeList02(T02, T03) >
#define TypeList04(T01, T02, T03, T04) \
	TypeList<T01, TypeList03(T02, T03, T04) >
#define TypeList05(T01, T02, T03, T04, T05) \
	TypeList<T01, TypeList04(T02, T03, T04, T05) >
#define TypeList06(T01, T02, T03, T04, T05, T06) \
	TypeList<T01, TypeList05(T02, T03, T04, T05, T06) >
#define TypeList07(T01, T02, T03, T04, T05, T06, T07) \
	TypeList<T01, TypeList06(T02, T03, T04, T05, T06, T07) >
#define TypeList08(T01, T02, T03, T04, T05, T06, T07, T08) \
	TypeList<T01, TypeList07(T02, T03, T04, T05, T06, T07, T08) >
#define TypeList09(T01, T02, T03, T04, T05, T06, T07, T08, T09) \
	TypeList<T01, TypeList08(T02, T03, T04, T05, T06, T07, T08, T09) >
#define TypeList10(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10) \
	TypeList<T01, TypeList09(T02, T03, T04, T05, T06, T07, T08, T09, T10) >
#define TypeList11(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11) \
	TypeList<T01, TypeList10(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11) >
#define TypeList12(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12) \
	TypeList<T01, TypeList11(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12) >
#define TypeList13(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13) \
	TypeList<T01, TypeList12(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13) >
#define TypeList14(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14) \
	TypeList<T01, TypeList13(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14) >
#define TypeList15(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15) \
	TypeList<T01, TypeList14(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15) >
#define TypeList16(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16) \
	TypeList<T01, TypeList15(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16) >
#define TypeList17(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17) \
	TypeList<T01, TypeList16(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17) >
#define TypeList18(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18) \
	TypeList<T01, TypeList17(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18) >
#define TypeList19(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19) \
	TypeList<T01, TypeList18(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19) >
#define TypeList20(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20) \
	TypeList<T01, TypeList19(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20) >
#define TypeList21(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21) \
	TypeList<T01, TypeList20(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21) >
#define TypeList22(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22) \
	TypeList<T01, TypeList21(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22) >
#define TypeList23(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23) \
	TypeList<T01, TypeList22(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23) >
#define TypeList24(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24) \
	TypeList<T01, TypeList23(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24) >
#define TypeList25(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25) \
	TypeList<T01, TypeList24(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25) >
#define TypeList26(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26) \
	TypeList<T01, TypeList25(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26) >
#define TypeList27(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27) \
	TypeList<T01, TypeList26(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27) >
#define TypeList28(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28) \
	TypeList<T01, TypeList27(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28) >
#define TypeList29(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29) \
	TypeList<T01, TypeList28(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29) >
#define TypeList30(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30) \
	TypeList<T01, TypeList29(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30) >
#define TypeList31(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31) \
	TypeList<T01, TypeList30(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31) >
#define TypeList32(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32) \
	TypeList<T01, TypeList31(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32) >
#define TypeList33(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33) \
	TypeList<T01, TypeList32(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33) >
#define TypeList34(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34) \
	TypeList<T01, TypeList33(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34) >
#define TypeList35(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35) \
	TypeList<T01, TypeList34(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35) >
#define TypeList36(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36) \
	TypeList<T01, TypeList35(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36) >
#define TypeList37(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37) \
	TypeList<T01, TypeList36(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37) >
#define TypeList38(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38) \
	TypeList<T01, TypeList37(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38) >
#define TypeList39(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39) \
	TypeList<T01, TypeList38(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39) >
#define TypeList40(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40) \
	TypeList<T01, TypeList39(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40) >
#define TypeList41(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41) \
	TypeList<T01, TypeList40(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41) >
#define TypeList42(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42) \
	TypeList<T01, TypeList41(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42) >
#define TypeList43(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43) \
	TypeList<T01, TypeList42(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43) >
#define TypeList44(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44) \
	TypeList<T01, TypeList43(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44) >
#define TypeList45(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45) \
	TypeList<T01, TypeList44(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45) >
#define TypeList46(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46) \
	TypeList<T01, TypeList45(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46) >
#define TypeList47(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47) \
	TypeList<T01, TypeList46(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47) >
#define TypeList48(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48) \
	TypeList<T01, TypeList47(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48) >
#define TypeList49(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49) \
	TypeList<T01, TypeList48(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49) >
#define TypeList50(T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50) \
	TypeList<T01, TypeList49(T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50) >

// Length: タイプリストの長さを求める
template<typename TList> struct Length;
template<> struct Length<NullType>
{
	static const int value = 0;
};

template <typename T, typename U>
struct Length<TypeList<T, U> >
{
	static const int value = 1 + Length<U>::value;
};

// TypeAt: index 番目の型を返す
template <class TList, unsigned int index> struct TypeAt;
template <class Head, class Tail>
struct TypeAt<TypeList<Head, Tail>, 0>
{
    typedef Head Result;
};

template <class Head, class Tail, unsigned int i>
struct TypeAt<TypeList<Head, Tail>, i>
{
    typedef typename TypeAt<Tail, i - 1>::Result Result;
};

// IndexOf: 指定された型の位置を返す
template <class TList, class T> struct IndexOf;
template <class T>
struct IndexOf<NullType, T>
{
	static const int value = -1;
};

template <class T, class Tail>
struct IndexOf<TypeList<T, Tail>, T>
{
	static const int value = 0;
};

template <class Head, class Tail, class T>
struct IndexOf<TypeList<Head, Tail>, T>
{
private:
	static const int temp = IndexOf<Tail, T>::value;
public:
	static const int value = (temp == -1) ? -1 : 1 + temp;
};

// Append: タイプリストに型を連結する
template <class TList, class T> struct Append;

template <> struct Append<NullType, NullType>
{
	typedef NullType Result;
};

template <class T> struct Append<NullType, T>
{
	typedef TypeList01(T) Result;
};

template <class Head, class Tail>
struct Append<NullType, TypeList<Head, Tail> >
{
	typedef TypeList<Head, Tail> Result;
};

template <class Head, class Tail, class T>
struct Append<TypeList<Head, Tail>, T>
{
	typedef TypeList<Head, typename Append<Tail, T>::Result> Result;
};

// Erase: タイプリストから指定された型で最初に見つかったものを削除する
template <class TList, class T> struct Erase;

template <class T>
struct Erase<NullType, T>
{
	typedef NullType Result;
};

template <class T, class Tail>
struct Erase<TypeList<T, Tail>, T>
{
	typedef Tail Result;
};

template <class Head, class Tail, class T>
struct Erase<TypeList<Head, Tail>, T>
{
	typedef TypeList<Head, typename Erase<Tail, T>::Result> Result;
};

// EraseAll: タイプリストから指定された型をすべて削除する
template <class TList, class T> struct EraseAll;

template <class T>
struct EraseAll<NullType, T>
{
	typedef NullType Result;
};

template <class T, class Tail>
struct EraseAll<TypeList<T, Tail>, T>
{
	// リストの最後まで進めながら、型の削除を行う
	typedef typename EraseAll<Tail, T>::Result Result;
};

template <class Head, class Tail, class T>
struct EraseAll<TypeList<Head, Tail>, T>
{
	// リストの最後まで進めながら、型の削除を行う
	typedef TypeList<Head, typename EraseAll<Tail, T>::Result> Result;
};

// NoDuplicates: タイプリスト内の型の重複を削除する
template <class TList> struct NoDuplicates;

template <> struct NoDuplicates<NullType>
{
	typedef NullType Result;
};

template <class Head, class Tail>
struct NoDuplicates<TypeList<Head, Tail> >
{
private:
	typedef typename NoDuplicates<Tail>::Result L1;
	typedef typename Erase<L1, Head>::Result L2;
public:
	typedef TypeList<Head, L2> Result;
};

// Replace: タイプリスト内の型を置換する
template <class TList, class T, class U> struct Replace;

template <class T, class U>
struct Replace<NullType, T, U>
{
	typedef NullType Result;
};

template <class T, class Tail, class U>
struct Replace<TypeList<T, Tail>, T, U>
{
	typedef TypeList<U, Tail> Result;
};

template <class Head, class Tail, class T, class U>
struct Replace<TypeList<Head, Tail>, T, U>
{
    typedef TypeList<Head, typename Replace<Tail, T, U>::Result> Result;
};

#if 0
// MostDerived:
template<class TLlist, class T> struct MostDerived;

template <class T>
struct MostDerived<NullType, T>
{
	typedef T Result;
};

template <class Head, class Tail, class T>
struct MostDerived<TypeList<Head, Tail>, T>
{
private:
	typedef typename MostDerived<Tail, T>::Result Candidate;
public:
	typedef typename Select<SUPERSUBCLASS(Candidate, Head), Head, Candidate>::Result Result;
};

// DerivedToFront:
template <class T> struct DerivedToFront;

template <>
struct DerivedToFront<NullType>
{
	typedef NullType Result;
};

template <class Head, class Tail>
struct DerivedToFront<TypeList<Head, Tail> >
{
private:
	typedef typename MostDerived<Tail, Head>::Result TheMostDerived;
	typedef typename Replace<Tail, TheMostDerived, Head>::Result L;
public:
	typedef TypeList<TheMostDerived, L> Result;
};

#endif

// StrTypeList: タイプリストの型名一覧の文字列を返す
template <typename TList> struct StrTypeList;

template <typename T, typename U>
struct StrTypeList<TypeList<T, U> >
{
	static std::string getStr()
	{
		return std::string(typeid(T).name()) + " " + StrTypeList<U>::getStr();
	}
};

template <>
struct StrTypeList<NullType>
{
	static std::string getStr()
	{
		return "";
	}
};

/*
// StrTypeList2: タイプリストの型名一覧の文字列を返す
// static メンバを使う版。bcc5.9.3 ではリンクが通らない

template <typename TList> struct StrTypeList2;

template <typename T, typename U>
struct StrTypeList2<TypeList<T, U> >
{
	static const std::string name;
};

template <>
struct StrTypeList2<NullType>
{
	static const std::string name;
};

template <typename T, typename U>
const std::string StrTypeList2<TypeList<T, U> >::name = std::string(typeid(T).name()) + " " + StrTypeList2<U>::name;

// cpp に追い出してインスタンスが 1 度しか作られないようにせなあかん
const std::string StrTypeList2<NullType>::name = "";
*/

#endif

