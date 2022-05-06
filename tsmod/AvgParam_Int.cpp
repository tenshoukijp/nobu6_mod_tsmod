#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "VariableNameRegistAndJudge.h"

// ����1300�l�̕����̕��ϒq�d
byte nb6AvgParam_Int[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM] = {
	94,
	58,
	66,
	90,
	73,
	94,
	66,
	98,
	91,
	72,
	91,
	72,
	92,
	82,
	84,
	26,
	76,
	78,
	90,
	94,
	42,
	72,
	75,
	96,
	88,
	55,
	70,
	68,
	84,
	86,
	77,
	55,
	36,
	29,
	50,
	88,
	97,
	101,
	75,
	90,
	96,
	96,
	41,
	48,
	89,
	75,
	64,
	85,
	79,
	39,
	86,
	44,
	66,
	85,
	80,
	98,
	72,
	59,
	88,
	56,
	92,
	92,
	96,
	92,
	87,
	82,
	88,
	61,
	63,
	73,
	82,
	69,
	68,
	88,
	87,
	75,
	85,
	88,
	76,
	60,
	59,
	69,
	27,
	55,
	83,
	78,
	39,
	15,
	47,
	102,
	73,
	81,
	66,
	88,
	83,
	42,
	71,
	50,
	59,
	83,
	86,
	90,
	60,
	71,
	69,
	78,
	70,
	94,
	89,
	75,
	53,
	68,
	26,
	65,
	87,
	69,
	62,
	45,
	78,
	86,
	79,
	84,
	81,
	75,
	12,
	38,
	79,
	66,
	33,
	74,
	33,
	77,
	81,
	47,
	77,
	63,
	80,
	39,
	87,
	28,
	63,
	76,
	50,
	42,
	82,
	59,
	71,
	68,
	27,
	61,
	47,
	63,
	72,
	55,
	42,
	68,
	56,
	77,
	44,
	64,
	71,
	24,
	34,
	46,
	74,
	66,
	42,
	60,
	18,
	20,
	46,
	75,
	70,
	46,
	75,
	58,
	70,
	54,
	40,
	62,
	51,
	11,
	34,
	78,
	53,
	27,
	66,
	34,
	60,
	19,
	42,
	74,
	50,
	61,
	34,
	81,
	43,
	58,
	85,
	89,
	87,
	71,
	47,
	48,
	64,
	49,
	73,
	59,
	9,
	72,
	78,
	79,
	33,
	25,
	26,
	67,
	48,
	47,
	67,
	54,
	76,
	12,
	63,
	71,
	63,
	74,
	73,
	36,
	52,
	50,
	44,
	71,
	76,
	49,
	62,
	52,
	38,
	48,
	32,
	68,
	79,
	91,
	25,
	75,
	54,
	73,
	42,
	73,
	8,
	53,
	40,
	44,
	66,
	66,
	70,
	78,
	65,
	86,
	52,
	35,
	54,
	52,
	74,
	43,
	65,
	71,
	30,
	58,
	37,
	60,
	38,
	17,
	64,
	53,
	40,
	66,
	58,
	71,
	75,
	62,
	71,
	74,
	58,
	60,
	28,
	47,
	51,
	68,
	55,
	50,
	53,
	60,
	72,
	54,
	37,
	61,
	63,
	54,
	65,
	36,
	65,
	65,
	79,
	29,
	7,
	35,
	52,
	36,
	48,
	67,
	80,
	72,
	53,
	66,
	76,
	64,
	40,
	57,
	89,
	62,
	63,
	82,
	82,
	74,
	57,
	54,
	52,
	83,
	88,
	78,
	40,
	71,
	72,
	71,
	32,
	24,
	44,
	83,
	54,
	68,
	24,
	52,
	25,
	65,
	40,
	40,
	38,
	58,
	46,
	36,
	68,
	65,
	42,
	55,
	78,
	41,
	41,
	68,
	48,
	69,
	50,
	85,
	86,
	56,
	64,
	61,
	41,
	50,
	50,
	60,
	50,
	64,
	58,
	50,
	67,
	57,
	50,
	61,
	50,
	74,
	70,
	78,
	67,
	62,
	54,
	33,
	32,
	44,
	35,
	49,
	10,
	54,
	34,
	50,
	60,
	35,
	45,
	37,
	63,
	63,
	73,
	55,
	54,
	70,
	68,
	37,
	40,
	58,
	41,
	57,
	32,
	49,
	47,
	63,
	41,
	63,
	59,
	39,
	60,
	66,
	47,
	45,
	62,
	37,
	55,
	37,
	56,
	19,
	62,
	51,
	51,
	31,
	28,
	54,
	48,
	52,
	66,
	70,
	32,
	50,
	63,
	42,
	48,
	70,
	59,
	37,
	54,
	49,
	49,
	32,
	55,
	74,
	26,
	54,
	43,
	71,
	55,
	69,
	60,
	51,
	54,
	52,
	47,
	79,
	76,
	74,
	33,
	54,
	61,
	41,
	65,
	65,
	51,
	52,
	75,
	63,
	60,
	56,
	60,
	41,
	31,
	67,
	57,
	38,
	62,
	66,
	57,
	51,
	40,
	41,
	43,
	57,
	64,
	35,
	52,
	49,
	35,
	35,
	55,
	26,
	81,
	45,
	55,
	82,
	79,
	48,
	66,
	59,
	52,
	50,
	46,
	55,
	63,
	51,
	42,
	49,
	53,
	54,
	38,
	50,
	62,
	69,
	49,
	49,
	26,
	81,
	51,
	51,
	44,
	32,
	59,
	50,
	35,
	36,
	62,
	56,
	37,
	54,
	48,
	51,
	33,
	58,
	64,
	32,
	41,
	45,
	45,
	68,
	46,
	47,
	51,
	40,
	55,
	47,
	61,
	50,
	24,
	53,
	59,
	39,
	38,
	31,
	75,
	25,
	32,
	57,
	46,
	62,
	48,
	39,
	40,
	55,
	23,
	37,
	55,
	34,
	22,
	31,
	43,
	34,
	18,
	38,
	59,
	31,
	48,
	58,
	50,
	53,
	44,
	61,
	37,
	43,
	23,
	22,
	28,
	43,
	59,
	55,
	31,
	55,
	50,
	45,
	50,
	43,
	42,
	38,
	52,
	45,
	62,
	64,
	62,
	56,
	55,
	49,
	43,
	45,
	37,
	63,
	67,
	63,
	18,
	37,
	38,
	42,
	51,
	56,
	46,
	50,
	72,
	65,
	37,
	49,
	56,
	38,
	34,
	45,
	53,
	51,
	28,
	58,
	46,
	57,
	47,
	77,
	36,
	34,
	45,
	72,
	51,
	58,
	41,
	49,
	59,
	47,
	42,
	44,
	71,
	65,
	38,
	46,
	48,
	42,
	57,
	67,
	51,
	32,
	73,
	68,
	41,
	74,
	43,
	40,
	46,
	67,
	34,
	25,
	38,
	28,
	31,
	75,
	32,
	51,
	69,
	72,
	52,
	40,
	32,
	36,
	17,
	51,
	55,
	68,
	34,
	54,
	48,
	56,
	37,
	50,
	25,
	51,
	65,
	40,
	45,
	55,
	40,
	72,
	64,
	38,
	52,
	62,
	22,
	50,
	57,
	51,
	77,
	61,
	61,
	68,
	41,
	53,
	58,
	47,
	55,
	49,
	48,
	76,
	23,
	42,
	53,
	61,
	51,
	75,
	43,
	35,
	37,
	47,
	23,
	41,
	67,
	43,
	71,
	67,
	9,
	48,
	72,
	50,
	78,
	53,
	49,
	52,
	76,
	34,
	27,
	71,
	84,
	76,
	38,
	45,
	43,
	62,
	51,
	67,
	54,
	36,
	57,
	74,
	56,
	45,
	34,
	49,
	70,
	22,
	26,
	39,
	46,
	23,
	55,
	66,
	35,
	50,
	41,
	33,
	50,
	48,
	47,
	42,
	48,
	58,
	53,
	54,
	55,
	46,
	51,
	50,
	69,
	68,
	45,
	66,
	33,
	60,
	54,
	47,
	52,
	42,
	63,
	66,
	43,
	42,
	69,
	70,
	57,
	21,
	59,
	60,
	72,
	22,
	56,
	63,
	50,
	45,
	37,
	30,
	57,
	46,
	64,
	49,
	61,
	53,
	73,
	36,
	56,
	57,
	64,
	58,
	59,
	48,
	64,
	32,
	63,
	44,
	27,
	48,
	49,
	58,
	52,
	63,
	61,
	36,
	18,
	58,
	53,
	78,
	66,
	64,
	49,
	24,
	64,
	41,
	41,
	27,
	15,
	29,
	66,
	38,
	35,
	49,
	47,
	51,
	44,
	47,
	28,
	57,
	51,
	30,
	63,
	71,
	68,
	52,
	45,
	75,
	30,
	50,
	39,
	42,
	56,
	33,
	46,
	73,
	44,
	45,
	47,
	70,
	52,
	52,
	15,
	31,
	56,
	43,
	38,
	64,
	28,
	45,
	26,
	75,
	85,
	84,
	54,
	56,
	24,
	30,
	49,
	72,
	58,
	36,
	56,
	60,
	44,
	52,
	49,
	50,
	50,
	50,
	56,
	62,
	41,
	54,
	49,
	38,
	65,
	25,
	54,
	66,
	19,
	53,
	52,
	45,
	32,
	40,
	47,
	59,
	50,
	30,
	32,
	51,
	72,
	81,
	61,
	61,
	56,
	31,
	26,
	64,
	73,
	59,
	52,
	42,
	78,
	58,
	58,
	49,
	59,
	32,
	66,
	33,
	55,
	36,
	41,
	54,
	25,
	27,
	45,
	63,
	40,
	50,
	56,
	44,
	62,
	43,
	47,
	64,
	56,
	61,
	77,
	46,
	70,
	54,
	62,
	51,
	37,
	68,
	53,
	27,
	67,
	51,
	79,
	61,
	52,
	81,
	89,
	74,
	57,
	48,
	37,
	56,
	84,
	68,
	38,
	73,
	75,
	29,
	73,
	46,
	26,
	71,
	59,
	41,
	55,
	76,
	67,
	36,
	69,
	15,
	68,
	49,
	36,
	67,
	23,
	40,
	13,
	67,
	27,
	69,
	85,
	70,
	54,
	50,
	70,
	59,
	69,
	44,
	31,
	57,
	57,
	59,
	53,
	37,
	50,
	26,
	50,
	73,
	59,
	51,
	51,
	53,
	37,
	33,
	26,
	34,
	57,
	28,
	20,
	56,
	46,
	46,
	47,
	61,
	22,
	40,
	44,
	61,
	42,
	47,
	12,
	51,
	47,
	48,
	42,
	50,
	46,
	24,
	49,
	53,
	47,
	48,
	45,
	59,
	45,
	49,
	47,
	55,
	58,
	33,
	51,
	53,
	48,
	11,
	42,
	58,
	42,
	48,
	31,
	33,
	72,
	61,
	71,
	37,
	69,
	50,
	70,
	54,
	57,
	31,
	31,
	50,
	61,
	48,
	49,
	40,
	47,
	62,
	62,
	75,
	36,
	41,
	44,
	74,
	48,
	43,
	42,
	53,
	48,
	31,
	45,
	51,
	31,
	18,
	31,
	37,
	45,
	41,
	40,
	73,
	35,
	29,
	21,
	55,
	47,
	59,
	35,
	43,
	49,
	34,
	42,
	36,
	41,
	42,
	54,
	12,
	46,
	45,
	51,
	44,
	45,
	33,
	60,
	15,
	62,
	31,
	32,
	50,
	48,
	46,
	32,
	48,
	40,
	53,
	42,
	29,
	32,
	52,
	46,
	42,
	51,
	36,
	56,
	57,
	49,
	39,
	26,
	42,
	41,
	38,
	26,
	16,
	25,
	32,
	47,
	35,
	44,
	21,
	38,
	45,
	37,
	26,
	33,
	34,
	47,
	48,
	42,
	43,
	29,
	58,
	36,
	40,
	44,
	14,
	47,
	41,
	45,
	12,
	46,
	44,
	36,
	48,
	46,
	51,
	46,
	37,
	23,
	31,
	27,
	39,
	39,
	42,
	40,
	27,
	41,
	39,
	22,
	56,
	48,
	52,
	34,
	27,
	30,
	37,
	35,
	55,
	32,
	61,
	46,
	39,
	38,
	57,
	26,
	56,
	35,
	53,
	61,
	69,
	54,
	61,
	49,
	35,
	64,
	50,
	21,
	28,
	56,
	37,
	19,
	49,
	44,
	39,
	49,
	54,
	65,
	60,
	38,
	44,
	59,
	52,
	44,
	41,
	80,
	56,
	39,
	49,
	55,
	55,
	58,
	46,
	48,
	45,
	65,
	58,
	61,
	25,
	68,
	50
};