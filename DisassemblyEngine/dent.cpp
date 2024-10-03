/****************************************************************************
*                                                                           *
* dent.cpp                                                                  *
*                                                                           *
* Copyright (c) 2024, Potato Corporation. All rights reserved.              *
*                                                                           *
****************************************************************************/

#include "dent.h"

// 全局标志
int g_nIsFirstLaunch = NULL;

// --------------------------------------------------------------------
// Table A-2表 One-byte Opcode
TABLE_A_2_INSTRUCTION_STRUCT pTable_A_2 = { 0 };

// Table 2-2表 32-Bit Addressing Forms with the ModR/M Byte
// 决定E的二维数组,Mod_R_M中MOD的值为0/1/2(32位/8位)
PCHAR Table_ModR_M[3][8] = {
	{"[EAX]", "[ECX]", "[EDX]", "[EBX]", "[[--][--]]", "[disp32]", "[ESI]", "[EDI]"},	// 无偏移
	{"EAX", "ECX", "EDX", "EBX", "[[--][--]+disp8]", "EBP", "ESI", "EDI"},				// 亦可以8位和32位共用
	{"EAX", "ECX", "EDX", "EBX", "[[--][--]+disp32]", "EBP", "ESI", "EDI"},				// 这里可以改......
};

// 决定E的二维数组,当MOD是0x3 寄存器的值(32位/8位)
PCHAR Table_ModR_M_Register[8][2] = {
	{"EAX", "AL"}, {"ECX", "CL"}, {"EDX", "DL"}, {"EBX", "BL"}, {"ESP", "AH"}, {"EBP", "CH"}, {"ESI", "DH"}, {"EDI", "BH"}
};

// 决定G的二维数组
PCHAR Table_ModR_M_Reg_Opcode[2][8] = {
	{"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"},
	{"AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH"},
};

// SIB_BASE
PCHAR Table_Sib_Base[8] = {"EAX", "ECX", "EDX", "EBX", "ESP", "[*]", "ESI", "EDI"};

// SIB二维数组
PCHAR Table_Scale_Index_Base[4][8] = {
	{"EAX", "ECX", "EDX", "EBX", "none", "EBP", "ESI", "EDI"},
	{"EAX*2", "ECX*2", "EDX*2", "EBX*2", "none", "EBP*2", "ESI*2", "EDI*2"},
	{"EAX*4", "ECX*4", "EDX*4", "EBX*4", "none", "EBP*4", "ESI*4", "EDI*4"},
	{"EAX*8", "ECX*8", "EDX*8", "EBX*8", "none", "EBP*8", "ESI*8", "EDI*8"},
};
// --------------------------------------------------------------------

// 单例模式
CHECK_PROCESS::_CHECK_PROCESS(PCHAR pszFlag)
{
	if (g_nIsFirstLaunch++ != NULL)
	{
		return;
	}
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, pszFlag);
	if (hMutex != NULL)
	{
		CloseHandle(hMutex);
		ExitProcess(0);
	}
	if (!CreateMutex(NULL, FALSE, pszFlag))
	{
		return;
	}
	return;
}

// --------------------------------------------------------------------

// 异常处理
EXCEPTION::_EXCEPTION()
{
	AddVectoredExceptionHandler(NULL, VectoredExceptionHandler);
	return;
}

// --------------------------------------------------------------------

// 初始化成员变量
VOID WINAPI DEBOOT::Init()
{
	this -> m_dwSignature = PE_SIGNATURE_INIT;
	this -> m_dwSizeOfFile = PE_SIGNATURE_INIT;
	this -> m_dwSizeOfImage = PE_SIGNATURE_INIT;
	this -> m_dwImageBase = PE_SIGNATURE_INIT;
	this -> m_dwImageBuffer = PE_SIGNATURE_INIT;
	this -> m_dwAddressOfEntryPoint = PE_SIGNATURE_INIT;
	this -> m_dwSizeOfCode = PE_SIGNATURE_INIT;
	return;
}

// 可执行程序检测
DEBOOT::_DEBOOT(IN int argc, IN _TCHAR* argv[])
{
	Init();
	// 是否仅输入了一个参数
	if (argc != EXIST_STRING)
	{
		this -> m_dwSignature = ERROR_PE_PARAM_MISTAKE;
		return;
	}
	// 是否是要求的文件类型
	if(IsPeFile32(
		argv[1], 
		&this -> m_dwSizeOfFile, 
		&this -> m_dwSizeOfImage, 
		&this -> m_dwImageBase) 
		== RETURN_ERROR)
	{
		this -> m_dwSignature = ERROR_PE_FILE_MISTAKE;
		return;
	}
}

// 可执行程序加载
VOID WINAPI DEBOOT::SDeBootLoad(IN _TCHAR* argv[])
{
	int nRet = 0;
	// 拉伸并加载可执行程序到内存
	nRet = LoadPeFile32(
		argv[1], 
		this -> m_dwSizeOfFile, 
		this -> m_dwSizeOfImage, 
		&this -> m_dwImageBuffer,
		&this -> m_dwAddressOfEntryPoint, 
		&this -> m_dwSizeOfCode
		);
	if(nRet)
	{
		this -> m_dwSignature = ERROR_PE_LOAD_MISTAKE;
	}
	return;
}

// 释放申请的空间
DEBOOT::~_DEBOOT()
{
	free((LPVOID)this -> m_dwImageBuffer);
	this -> m_dwSignature = NULL;
	this -> m_dwSizeOfFile = NULL;
	this -> m_dwSizeOfImage = NULL;
	this -> m_dwImageBase = NULL;
	this -> m_dwImageBuffer = NULL;
	this -> m_dwAddressOfEntryPoint = NULL;
	this -> m_dwSizeOfCode = NULL;
	return;
}

// --------------------------------------------------------------------

// 初始化成员变量
VOID WINAPI DIS_ASSEMBLY_ENGINE::Init()
{
	this -> m_dwPointerToTable_2_2 = NULL;
	this -> m_dwPointerToTable_2_3 = NULL;
	this -> m_dwPointerToTable_A_2 = NULL;
	this -> m_dwPointerToTable_A_3 = NULL;
	this -> m_dwPointerToTable_A_4 = NULL;
	this -> m_dwPointerToTable_A_5 = NULL;
	this -> m_dwPointerToTable_A_6 = NULL;
	return;
}

// 自定义数据结构内存展开
DIS_ASSEMBLY_ENGINE::_DIS_ASSEMBLY_ENGINE()
{
	Init();
	BOOL nRet = 0;
	nRet = this -> Table_A_2_Init();
	return;
}

// Table A-2初始化
BOOL WINAPI DIS_ASSEMBLY_ENGINE::Table_A_2_Init()
{
	// ADD
	pTable_A_2.Table_A_2_InstructionStruct[0].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0].m_bInstructionFormat = FORMAT_EB_GB;	// Eb, Gb
	pTable_A_2.Table_A_2_InstructionStruct[0].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0].pszInstructionString = "ADD";

	pTable_A_2.Table_A_2_InstructionStruct[1].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[1].m_bInstructionFormat = FORMAT_EV_GV;	// Ev, Gv
	pTable_A_2.Table_A_2_InstructionStruct[1].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[1].pszInstructionString = "ADD";

	pTable_A_2.Table_A_2_InstructionStruct[2].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[2].m_bInstructionFormat = FORMAT_GB_EB;	// Gb, Eb
	pTable_A_2.Table_A_2_InstructionStruct[2].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[2].pszInstructionString = "ADD";

	pTable_A_2.Table_A_2_InstructionStruct[3].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[3].m_bInstructionFormat = FORMAT_GV_EV;	// Gv, Ev
	pTable_A_2.Table_A_2_InstructionStruct[3].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[3].pszInstructionString = "ADD";

	pTable_A_2.Table_A_2_InstructionStruct[4].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[4].m_bInstructionFormat = FORMAT_AL_IB;	// AL, Ib
	pTable_A_2.Table_A_2_InstructionStruct[4].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[4].pszInstructionString = "ADD";

	pTable_A_2.Table_A_2_InstructionStruct[5].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[5].m_bInstructionFormat = FORMAT_RAX_IZ;	// rAX, Iz
	pTable_A_2.Table_A_2_InstructionStruct[5].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[5].pszInstructionString = "ADD";

	// PUSH/POP
	pTable_A_2.Table_A_2_InstructionStruct[6].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[6].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[6].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[6].pszInstructionString = "PUSH ES";

	pTable_A_2.Table_A_2_InstructionStruct[7].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[7].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[7].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[7].pszInstructionString = "POP ES";

	// OR
	pTable_A_2.Table_A_2_InstructionStruct[8].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[8].m_bInstructionFormat = FORMAT_EB_GB;	// Eb, Gb
	pTable_A_2.Table_A_2_InstructionStruct[8].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[8].pszInstructionString = "OR";

	pTable_A_2.Table_A_2_InstructionStruct[9].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[9].m_bInstructionFormat = FORMAT_EV_GV;	// Ev, Gv
	pTable_A_2.Table_A_2_InstructionStruct[9].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[9].pszInstructionString = "OR";

	pTable_A_2.Table_A_2_InstructionStruct[0xA].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0xA].m_bInstructionFormat = FORMAT_GB_EB;	// Gb, Eb
	pTable_A_2.Table_A_2_InstructionStruct[0xA].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0xA].pszInstructionString = "OR";

	pTable_A_2.Table_A_2_InstructionStruct[0xB].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0xB].m_bInstructionFormat = FORMAT_GV_EV;	// Gv, Ev
	pTable_A_2.Table_A_2_InstructionStruct[0xB].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0xB].pszInstructionString = "OR";

	pTable_A_2.Table_A_2_InstructionStruct[0xC].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0xC].m_bInstructionFormat = FORMAT_AL_IB;	// AL, Ib
	pTable_A_2.Table_A_2_InstructionStruct[0xC].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0xC].pszInstructionString = "OR";

	pTable_A_2.Table_A_2_InstructionStruct[0xD].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0xD].m_bInstructionFormat = FORMAT_RAX_IZ;	// rAX, Iz
	pTable_A_2.Table_A_2_InstructionStruct[0xD].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0xD].pszInstructionString = "OR";

	// PUSH
	pTable_A_2.Table_A_2_InstructionStruct[0xE].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0xE].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0xE].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0xE].pszInstructionString = "PUSH CS";

	// -----------------------------------------------------------------------------------------
	// 要处理0x0F...系列指令需要建立Table A-3
	// 暂不处理
	// -----------------------------------------------------------------------------------------

	// ADC
	pTable_A_2.Table_A_2_InstructionStruct[0x10].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x10].m_bInstructionFormat = FORMAT_EB_GB;// Eb, Gb
	pTable_A_2.Table_A_2_InstructionStruct[0x10].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x10].pszInstructionString = "ADC";

	pTable_A_2.Table_A_2_InstructionStruct[0x11].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x11].m_bInstructionFormat = FORMAT_EV_GV;// Ev, Gv
	pTable_A_2.Table_A_2_InstructionStruct[0x11].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x11].pszInstructionString = "ADC";

	pTable_A_2.Table_A_2_InstructionStruct[0x12].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x12].m_bInstructionFormat = FORMAT_GB_EB;// Gb, Eb
	pTable_A_2.Table_A_2_InstructionStruct[0x12].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x12].pszInstructionString = "ADC";

	pTable_A_2.Table_A_2_InstructionStruct[0x13].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x13].m_bInstructionFormat = FORMAT_GV_EV;// Gv, Ev
	pTable_A_2.Table_A_2_InstructionStruct[0x13].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x13].pszInstructionString = "ADC";

	pTable_A_2.Table_A_2_InstructionStruct[0x14].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x14].m_bInstructionFormat = FORMAT_AL_IB;	// AL, Ib
	pTable_A_2.Table_A_2_InstructionStruct[0x14].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x14].pszInstructionString = "ADC";

	pTable_A_2.Table_A_2_InstructionStruct[0x15].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x15].m_bInstructionFormat = FORMAT_RAX_IZ;	// rAX, Iz
	pTable_A_2.Table_A_2_InstructionStruct[0x15].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x15].pszInstructionString = "ADC";

	// PUSH/POP
	pTable_A_2.Table_A_2_InstructionStruct[0x16].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x16].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x16].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x16].pszInstructionString = "PUSH SS";

	pTable_A_2.Table_A_2_InstructionStruct[0x17].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x17].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x17].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x17].pszInstructionString = "POP SS";

	// SBB
	pTable_A_2.Table_A_2_InstructionStruct[0x18].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x18].m_bInstructionFormat = FORMAT_EB_GB;// Eb, Gb
	pTable_A_2.Table_A_2_InstructionStruct[0x18].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x18].pszInstructionString = "SBB";

	pTable_A_2.Table_A_2_InstructionStruct[0x19].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x19].m_bInstructionFormat = FORMAT_EV_GV;// Ev, Gv
	pTable_A_2.Table_A_2_InstructionStruct[0x19].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x19].pszInstructionString = "SBB";

	pTable_A_2.Table_A_2_InstructionStruct[0x1A].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x1A].m_bInstructionFormat = FORMAT_GB_EB;// Gb, Eb
	pTable_A_2.Table_A_2_InstructionStruct[0x1A].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x1A].pszInstructionString = "SBB";

	pTable_A_2.Table_A_2_InstructionStruct[0x1B].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x1B].m_bInstructionFormat = FORMAT_GV_EV;// Gv, Ev
	pTable_A_2.Table_A_2_InstructionStruct[0x1B].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x1B].pszInstructionString = "SBB";

	pTable_A_2.Table_A_2_InstructionStruct[0x1C].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x1C].m_bInstructionFormat = FORMAT_AL_IB;	// AL, Ib
	pTable_A_2.Table_A_2_InstructionStruct[0x1C].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x1C].pszInstructionString = "SBB";

	pTable_A_2.Table_A_2_InstructionStruct[0x1D].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x1D].m_bInstructionFormat = FORMAT_RAX_IZ;	// rAX, Iz
	pTable_A_2.Table_A_2_InstructionStruct[0x1D].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x1D].pszInstructionString = "SBB";

	// PUSH/POP
	pTable_A_2.Table_A_2_InstructionStruct[0x1E].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x1E].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x1E].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x1E].pszInstructionString = "PUSH DS";

	pTable_A_2.Table_A_2_InstructionStruct[0x1F].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x1F].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x1F].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x1F].pszInstructionString = "POP DS";

	// AND
	pTable_A_2.Table_A_2_InstructionStruct[0x20].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x20].m_bInstructionFormat = FORMAT_EB_GB;// Eb, Gb
	pTable_A_2.Table_A_2_InstructionStruct[0x20].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x20].pszInstructionString = "AND";

	pTable_A_2.Table_A_2_InstructionStruct[0x21].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x21].m_bInstructionFormat = FORMAT_EV_GV;// Ev, Gv
	pTable_A_2.Table_A_2_InstructionStruct[0x21].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x21].pszInstructionString = "AND";

	pTable_A_2.Table_A_2_InstructionStruct[0x22].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x22].m_bInstructionFormat = FORMAT_GB_EB;// Gb, Eb
	pTable_A_2.Table_A_2_InstructionStruct[0x22].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x22].pszInstructionString = "AND";

	pTable_A_2.Table_A_2_InstructionStruct[0x23].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x23].m_bInstructionFormat = FORMAT_GV_EV;// Gv, Ev
	pTable_A_2.Table_A_2_InstructionStruct[0x23].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x23].pszInstructionString = "AND";

	pTable_A_2.Table_A_2_InstructionStruct[0x24].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x24].m_bInstructionFormat = FORMAT_AL_IB;	// AL, Ib
	pTable_A_2.Table_A_2_InstructionStruct[0x24].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x24].pszInstructionString = "AND";

	pTable_A_2.Table_A_2_InstructionStruct[0x25].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x25].m_bInstructionFormat = FORMAT_RAX_IZ;	// rAX, Iz
	pTable_A_2.Table_A_2_InstructionStruct[0x25].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x25].pszInstructionString = "AND";

	// -----------------------------------------------------------------------------------------
	// 0x26 Prefix 前缀指令
	// 暂不处理
	// -----------------------------------------------------------------------------------------

	// DAA
	pTable_A_2.Table_A_2_InstructionStruct[0x27].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x27].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x27].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x27].pszInstructionString = "DAA";

	// SUB
	pTable_A_2.Table_A_2_InstructionStruct[0x28].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x28].m_bInstructionFormat = FORMAT_EB_GB;// Eb, Gb
	pTable_A_2.Table_A_2_InstructionStruct[0x28].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x28].pszInstructionString = "SUB";

	pTable_A_2.Table_A_2_InstructionStruct[0x29].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x29].m_bInstructionFormat = FORMAT_EV_GV;// Ev, Gv
	pTable_A_2.Table_A_2_InstructionStruct[0x29].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x29].pszInstructionString = "SUB";

	pTable_A_2.Table_A_2_InstructionStruct[0x2A].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x2A].m_bInstructionFormat = FORMAT_GB_EB;// Gb, Eb
	pTable_A_2.Table_A_2_InstructionStruct[0x2A].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x2A].pszInstructionString = "SUB";

	pTable_A_2.Table_A_2_InstructionStruct[0x2B].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x2B].m_bInstructionFormat = FORMAT_GV_EV;// Gv, Ev
	pTable_A_2.Table_A_2_InstructionStruct[0x2B].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x2B].pszInstructionString = "SUB";

	pTable_A_2.Table_A_2_InstructionStruct[0x2C].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x2C].m_bInstructionFormat = FORMAT_AL_IB;	// AL, Ib
	pTable_A_2.Table_A_2_InstructionStruct[0x2C].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x2C].pszInstructionString = "SUB";

	pTable_A_2.Table_A_2_InstructionStruct[0x2D].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x2D].m_bInstructionFormat = FORMAT_RAX_IZ;	// rAX, Iz
	pTable_A_2.Table_A_2_InstructionStruct[0x2D].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x2D].pszInstructionString = "SUB";

	// -----------------------------------------------------------------------------------------
	// 0x2E Prefix 前缀指令
	// 暂不处理
	// -----------------------------------------------------------------------------------------

	// DAS
	pTable_A_2.Table_A_2_InstructionStruct[0x2F].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x2F].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x2F].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x2F].pszInstructionString = "DAS";

	// XOR
	pTable_A_2.Table_A_2_InstructionStruct[0x30].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x30].m_bInstructionFormat = FORMAT_EB_GB;// Eb, Gb
	pTable_A_2.Table_A_2_InstructionStruct[0x30].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x30].pszInstructionString = "XOR";

	pTable_A_2.Table_A_2_InstructionStruct[0x31].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x31].m_bInstructionFormat = FORMAT_EV_GV;// Ev, Gv
	pTable_A_2.Table_A_2_InstructionStruct[0x31].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x31].pszInstructionString = "XOR";

	pTable_A_2.Table_A_2_InstructionStruct[0x32].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x32].m_bInstructionFormat = FORMAT_GB_EB;// Gb, Eb
	pTable_A_2.Table_A_2_InstructionStruct[0x32].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x32].pszInstructionString = "XOR";

	pTable_A_2.Table_A_2_InstructionStruct[0x33].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x33].m_bInstructionFormat = FORMAT_GV_EV;// Gv, Ev
	pTable_A_2.Table_A_2_InstructionStruct[0x33].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x33].pszInstructionString = "XOR";

	pTable_A_2.Table_A_2_InstructionStruct[0x34].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x34].m_bInstructionFormat = FORMAT_AL_IB;	// AL, Ib
	pTable_A_2.Table_A_2_InstructionStruct[0x34].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x34].pszInstructionString = "XOR";

	pTable_A_2.Table_A_2_InstructionStruct[0x35].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x35].m_bInstructionFormat = FORMAT_RAX_IZ;	// rAX, Iz
	pTable_A_2.Table_A_2_InstructionStruct[0x35].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x35].pszInstructionString = "XOR";

	// -----------------------------------------------------------------------------------------
	// 0x36 Prefix 前缀指令
	// 暂不处理
	// -----------------------------------------------------------------------------------------

	// AAA
	pTable_A_2.Table_A_2_InstructionStruct[0x37].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x37].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x37].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x37].pszInstructionString = "AAA";

	// CMP
	pTable_A_2.Table_A_2_InstructionStruct[0x38].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x38].m_bInstructionFormat = FORMAT_EB_GB;// Eb, Gb
	pTable_A_2.Table_A_2_InstructionStruct[0x38].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x38].pszInstructionString = "CMP";

	pTable_A_2.Table_A_2_InstructionStruct[0x39].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x39].m_bInstructionFormat = FORMAT_EV_GV;// Ev, Gv
	pTable_A_2.Table_A_2_InstructionStruct[0x39].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x39].pszInstructionString = "CMP";

	pTable_A_2.Table_A_2_InstructionStruct[0x3A].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x3A].m_bInstructionFormat = FORMAT_GB_EB;// Gb, Eb
	pTable_A_2.Table_A_2_InstructionStruct[0x3A].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x3A].pszInstructionString = "CMP";

	pTable_A_2.Table_A_2_InstructionStruct[0x3B].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x3B].m_bInstructionFormat = FORMAT_GV_EV;// Gv, Ev
	pTable_A_2.Table_A_2_InstructionStruct[0x3B].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x3B].pszInstructionString = "CMP";

	pTable_A_2.Table_A_2_InstructionStruct[0x3C].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x3C].m_bInstructionFormat = FORMAT_AL_IB;	// AL, Ib
	pTable_A_2.Table_A_2_InstructionStruct[0x3C].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x3C].pszInstructionString = "CMP";

	pTable_A_2.Table_A_2_InstructionStruct[0x3D].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x3D].m_bInstructionFormat = FORMAT_RAX_IZ;	// rAX, Iz
	pTable_A_2.Table_A_2_InstructionStruct[0x3D].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x3D].pszInstructionString = "CMP";

	// -----------------------------------------------------------------------------------------
	// 0x3E Prefix 前缀指令
	// 暂不处理
	// -----------------------------------------------------------------------------------------

	// AAS
	pTable_A_2.Table_A_2_InstructionStruct[0x3F].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x3F].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x3F].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x3F].pszInstructionString = "AAS";

	// INC
	pTable_A_2.Table_A_2_InstructionStruct[0x40].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x40].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x40].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x40].pszInstructionString = "INC EAX";

	pTable_A_2.Table_A_2_InstructionStruct[0x41].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x41].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x41].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x41].pszInstructionString = "INC ECX";

	pTable_A_2.Table_A_2_InstructionStruct[0x42].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x42].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x42].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x42].pszInstructionString = "INC EDX";

	pTable_A_2.Table_A_2_InstructionStruct[0x43].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x43].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x43].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x43].pszInstructionString = "INC EBX";

	pTable_A_2.Table_A_2_InstructionStruct[0x44].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x44].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x44].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x44].pszInstructionString = "INC ESP";

	pTable_A_2.Table_A_2_InstructionStruct[0x45].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x45].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x45].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x45].pszInstructionString = "INC EBP";

	pTable_A_2.Table_A_2_InstructionStruct[0x46].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x46].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x46].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x46].pszInstructionString = "INC ESI";

	pTable_A_2.Table_A_2_InstructionStruct[0x47].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x47].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x47].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x47].pszInstructionString = "INC EDI";

	// DEC
	pTable_A_2.Table_A_2_InstructionStruct[0x48].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x48].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x48].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x48].pszInstructionString = "DEC EAX";

	pTable_A_2.Table_A_2_InstructionStruct[0x49].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x49].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x49].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x49].pszInstructionString = "DEC ECX";

	pTable_A_2.Table_A_2_InstructionStruct[0x4A].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x4A].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x4A].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x4A].pszInstructionString = "DEC EDX";

	pTable_A_2.Table_A_2_InstructionStruct[0x4B].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x4B].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x4B].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x4B].pszInstructionString = "DEC EBX";

	pTable_A_2.Table_A_2_InstructionStruct[0x4C].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x4C].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x4C].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x4C].pszInstructionString = "DEC ESP";

	pTable_A_2.Table_A_2_InstructionStruct[0x4D].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x4D].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x4D].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x4D].pszInstructionString = "DEC EBP";

	pTable_A_2.Table_A_2_InstructionStruct[0x4E].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x4E].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x4E].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x4E].pszInstructionString = "DEC ESI";

	pTable_A_2.Table_A_2_InstructionStruct[0x4F].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x4F].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x4F].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x4F].pszInstructionString = "DEC EDI";

	// PUSH
	pTable_A_2.Table_A_2_InstructionStruct[0x50].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x50].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x50].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x50].pszInstructionString = "PUSH EAX";

	pTable_A_2.Table_A_2_InstructionStruct[0x51].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x51].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x51].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x51].pszInstructionString = "PUSH ECX";

	pTable_A_2.Table_A_2_InstructionStruct[0x52].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x52].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x52].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x52].pszInstructionString = "PUSH EDX";

	pTable_A_2.Table_A_2_InstructionStruct[0x53].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x53].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x53].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x53].pszInstructionString = "PUSH EBX";

	pTable_A_2.Table_A_2_InstructionStruct[0x54].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x54].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x54].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x54].pszInstructionString = "PUSH ESP";

	pTable_A_2.Table_A_2_InstructionStruct[0x55].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x55].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x55].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x55].pszInstructionString = "PUSH EBP";

	pTable_A_2.Table_A_2_InstructionStruct[0x56].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x56].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x56].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x56].pszInstructionString = "PUSH ESI";

	pTable_A_2.Table_A_2_InstructionStruct[0x57].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x57].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x57].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x57].pszInstructionString = "PUSH EDI";

	// POP
	pTable_A_2.Table_A_2_InstructionStruct[0x58].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x58].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x58].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x58].pszInstructionString = "POP EAX";

	pTable_A_2.Table_A_2_InstructionStruct[0x59].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x59].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x59].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x59].pszInstructionString = "POP ECX";

	pTable_A_2.Table_A_2_InstructionStruct[0x5A].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x5A].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x5A].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x5A].pszInstructionString = "POP EDX";

	pTable_A_2.Table_A_2_InstructionStruct[0x5B].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x5B].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x5B].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x5B].pszInstructionString = "POP EBX";

	pTable_A_2.Table_A_2_InstructionStruct[0x5C].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x5C].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x5C].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x5C].pszInstructionString = "POP ESP";

	pTable_A_2.Table_A_2_InstructionStruct[0x5D].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x5D].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x5D].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x5D].pszInstructionString = "POP EBP";

	pTable_A_2.Table_A_2_InstructionStruct[0x5E].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x5E].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x5E].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x5E].pszInstructionString = "POP ESI";

	pTable_A_2.Table_A_2_InstructionStruct[0x5F].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x5F].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x5F].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x5F].pszInstructionString = "POP EDI";

	// PUSHAD
	pTable_A_2.Table_A_2_InstructionStruct[0x60].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x60].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x60].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x60].pszInstructionString = "PUSHAD";

	// POPAD
	pTable_A_2.Table_A_2_InstructionStruct[0x61].m_bInstructionType = VARIABLE;
	pTable_A_2.Table_A_2_InstructionStruct[0x61].m_bInstructionFormat = FORMAT_FIXED_LENGTH;
	pTable_A_2.Table_A_2_InstructionStruct[0x61].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x61].pszInstructionString = "POPAD";

	// BOUND
	pTable_A_2.Table_A_2_InstructionStruct[0x62].m_bInstructionType = FIXED;
	pTable_A_2.Table_A_2_InstructionStruct[0x62].m_bInstructionFormat = FORMAT_GV_MA;	// Gv, Ma
	pTable_A_2.Table_A_2_InstructionStruct[0x62].m_bIsGroupNumber = FALSE;
	pTable_A_2.Table_A_2_InstructionStruct[0x62].pszInstructionString = "BOUND";
	// 这里需要先手动解析BOUND指令
	
	return RETURN_SUCCESS;
}

// Table 2-2初始化
BOOL WINAPI DIS_ASSEMBLY_ENGINE::Table_2_2_Init()
{
	return RETURN_SUCCESS;
}

// 反汇编引擎启动!!!
BOOL WINAPI DIS_ASSEMBLY_ENGINE::DisAssemblyAnalysis(
	IN DWORD dwImageBuffer, 
	IN DWORD dwAddressOfEntryPoint, 
	IN DWORD dwSizeOfCode, 
	IN DWORD dwImageBase
	)
{
	// 得到运行时应该处于的虚拟地址以显示
	DWORD dwAddress = dwImageBase + dwAddressOfEntryPoint;
	// 定位到.txt段
	PBYTE pszInstruction = (PBYTE)(dwAddressOfEntryPoint + dwImageBuffer);
todo:
	// 取一个字节并将其作为下标
	BYTE bInstructionIndex = *(BYTE*)pszInstruction;
	// 暂时作为跳出条件
	if (bInstructionIndex == 0xCC)
	{
		return RETURN_SUCCESS;
	}
	// 得到通用结构体
	PINSTRUCTION_STRUCT pRet = (PINSTRUCTION_STRUCT)&pTable_A_2.Table_A_2_InstructionStruct[bInstructionIndex];
	// 首先判断是否有Group字段
	if (pRet -> m_bIsGroupNumber == TRUE)
	{
		printf("有Group字段,查找Table A-6\n");
		goto todo;			// 这里不一定跳转
	}
	// 得到操作码
	PCHAR pszOpCode = pRet -> pszInstructionString;
	// 判断指令类型是否是变长/定长/前缀(若Opcode确定了指令的长度就确定了)
	// 前缀指令
	if(pRet -> m_bInstructionType == PREFIX)
	{
		printf("是前缀指令\n");
		goto todo;
	}
	// 定长指令
	if(pRet -> m_bInstructionType == VARIABLE)
	{
		switch(pRet -> m_bInstructionFormat)
		{
		case FORMAT_AL_IB:
			{
				// AL, Ib
				// 定长,取后1个字节作为偏移
				BYTE bDisp = *++pszInstruction;
				printf("%x   %s AL,%x\n", dwAddress, pszOpCode, bDisp);
				dwAddress += 2;
				pszInstruction++;
				break;
			}
		case FORMAT_RAX_IZ:
			{
				//  rAX, Iz
				// 定长,取后4个字节作为偏移
				DWORD dwDisp = *(LPDWORD)++pszInstruction;
				printf("%x   %s EAX,%x\n", dwAddress, pszOpCode, dwDisp);
				dwAddress += 5;
				pszInstruction += 4;
				break;
			}
		case FORMAT_FIXED_LENGTH:
			{
				// 固定长度指令无偏移
				printf("%x   %s\n", dwAddress, pszOpCode);
				dwAddress++;
				pszInstruction++;
				break;
			}
		default:
			break;
		}
		goto todo;
	}
	// 变长指令
	if (pRet -> m_bInstructionType == FIXED)
	{
		// 是变长则先得到MOD/RM(后一个字节)
		BYTE bModRm = *++pszInstruction;
		// Reg/Opcode决定G是什么, Mod + R/M决定E是什么
		// 因为没有Group字段所以Reg/Opcode代表的是Reg
		// 拆分MOD/RM
		BYTE MOD = GET_MOD(bModRm);
		BYTE Reg_Opcode = GET_REG_OPCODE(bModRm);
		BYTE R_M = GET_R_M(bModRm);
		// 定义根据指令格式动态变化的值
		// 操作数宽度
		BYTE bWidth = WIDTH_B;
		// 检测指令操作数宽度是否是32位 (Ev,Gv || Gv,Ev)
		if (pRet -> m_bInstructionFormat == FORMAT_EV_GV || 
			pRet -> m_bInstructionFormat == FORMAT_GV_EV ||
			pRet -> m_bInstructionFormat == FORMAT_GV_MA)
		{
			bWidth = WIDTH_V;
		}
		// 声明E和G
		PCHAR pszStringE = NULL;
		PCHAR pszStringG = NULL;
		// 找到G
		pszStringG = Table_ModR_M_Reg_Opcode[bWidth][Reg_Opcode];
		// Mod等于0且RM等于5
		if (MOD == MOD_MEMORY_ZERO && R_M == DISP_32)
		{
			DWORD dwDisp32 = *(LPDWORD)++pszInstruction;
			PrintFormat(MOD, R_M, NULL, NULL, pRet -> m_bInstructionFormat, NULL, NULL, NULL, dwAddress, 
				pszOpCode, NULL, NULL, pszStringG, NULL, NULL, dwDisp32, NULL);
			pszInstruction += 4;
			dwAddress += 6;
		}
		// Mod等于0且RM不等于4
		else if (MOD == MOD_MEMORY_ZERO && R_M != SIB_FOLLOW)
		{
			pszStringE = Table_ModR_M[MOD][R_M];		// 找到E -> Memory
			// 无偏移无SIB
			PrintFormat(MOD, R_M, NULL, NULL, pRet -> m_bInstructionFormat, NULL, NULL, NULL, dwAddress, 
				pszOpCode, NULL, pszStringE, pszStringG, NULL, NULL, NULL, NULL);
			dwAddress += 2;
			pszInstruction++;
		}
		// Mod等于1且RM不等于4
		else if (MOD == MOD_MEMORY_BYTE && R_M != SIB_FOLLOW)
		{
			// 8位偏移无SIB
			pszStringE = Table_ModR_M[MOD][R_M];		// 找到E -> Memory
			PCHAR pszSymbol = "+";
			BYTE bStringCode = *(PBYTE)(pszInstruction + 1);
			// 一字节立即数大于等于0x80则为负
			if (bStringCode >= 0x80)
			{
				bStringCode = 0x100 - bStringCode;
				pszSymbol = "-";
			}
			PrintFormat(MOD, R_M, NULL, NULL, pRet -> m_bInstructionFormat, bStringCode, NULL, NULL, dwAddress, 
				pszOpCode, pszSymbol, pszStringE, pszStringG, NULL, NULL, NULL, NULL);
			pszInstruction += 2;
			dwAddress += 3;
		}
		// Mod等于2且RM不等于4
		else if (MOD == MOD_MEMORY_DWORD && R_M != SIB_FOLLOW)
		{
			// 32位偏移无SIB
			pszStringE = Table_ModR_M[MOD][R_M];		// 找到E -> Memory
			PCHAR pszSymbol = "+";
			DWORD dwStringCode = *(PDWORD)(pszInstruction + 1);
			if (dwStringCode >= 0x80000000)
			{
				dwStringCode = 0x100000000 - dwStringCode;
				pszSymbol = "-";
			}
			PrintFormat(MOD, R_M, NULL, NULL, pRet -> m_bInstructionFormat, NULL, NULL, dwStringCode, dwAddress, 
				pszOpCode, pszSymbol, pszStringE, pszStringG, NULL, NULL, NULL, NULL);
			pszInstruction += 5;
			dwAddress += 6;
		}
		// MOD等于3则操作数一定是寄存器,此时不存在SIB并且也没有偏移
		else if (MOD == MOD_REGISTER)
		{
			pszStringE = Table_ModR_M_Register[R_M][bWidth];		// 找到E -> Register
			PrintFormat(MOD, R_M, NULL, NULL, pRet -> m_bInstructionFormat, NULL, NULL, NULL, dwAddress, 
				pszOpCode, NULL, pszStringE, pszStringG, NULL, NULL, NULL, NULL);
			dwAddress += 2;
			pszInstruction++;
		}
		// Mod等于0且RM等于4
		else if (MOD == MOD_MEMORY_ZERO && R_M == SIB_FOLLOW)
		{
			// 无偏移有SIB
			// 首先获得SIB的字节
			BYTE bSib = *++pszInstruction;
			// 拆分SIB
			BYTE Scale = GET_SCALE(bSib);
			BYTE Index = GET_INDEX(bSib);
			BYTE Base = GET_BASE(bSib);
			// Base + Index*2(^Scale)  (SIB也确定E)
			PCHAR pszBase = Table_Sib_Base[Base];
			PCHAR pszScaleIndex = Table_Scale_Index_Base[Scale][Index];
			// [scaled index] + disp32 
			if (Base == SIB_BASE)
			{
				DWORD dwDisp = *(LPDWORD)++pszInstruction;
				PrintFormat(MOD, R_M, Base, Index, pRet -> m_bInstructionFormat, NULL, NULL, NULL, dwAddress, 
					pszOpCode, NULL, pszStringE, pszStringG, NULL, pszScaleIndex, NULL, dwDisp);
				pszInstruction += 4;
				dwAddress += 7;
			}
			else
			{
				PrintFormat(MOD, R_M, Base, Index, pRet -> m_bInstructionFormat, NULL, NULL, NULL, dwAddress, 
					pszOpCode, NULL, pszStringE, pszStringG, pszBase, pszScaleIndex, NULL, NULL);
				pszInstruction++;
				dwAddress += 3;
			}
		}
		// Mod等于1且RM等于4
		else if (MOD == MOD_MEMORY_BYTE && R_M == SIB_FOLLOW)
		{
			//----------------------------------------------------<<目前写到这里
			// SIB中,Base = 5（0b101）  时，有8位偏移的情况分析
			// PrintFormat函数参数填充
			//----------------------------------------------------<<目前写到这里  
			// 8位偏移有SIB
			// 先找SIB再找偏移
			BYTE bSib = *++pszInstruction;
			BYTE Scale = GET_SCALE(bSib);
			BYTE Index = GET_INDEX(bSib);
			BYTE Base = GET_BASE(bSib);
			// [scaled index] + disp8 + [EBP]
			PCHAR pszBase = Table_Sib_Base[Base];
			PCHAR pszScaleIndex = Table_Scale_Index_Base[Scale][Index];
			// 找偏移及正确的符号
			PCHAR pszSymbol = "+";
			BYTE bStringCode = *(PBYTE)(pszInstruction + 1);
			// 一字节立即数大于等于0x80则为负
			if (bStringCode >= 0x80)
			{
				bStringCode = 0x100 - bStringCode;
				pszSymbol = "-";
			}
			// [scaled index] + disp8 + [EBP]
			if (Base == SIB_BASE)
			{
				BYTE bDisp = *++pszInstruction;
				PrintFormat(MOD, R_M, Base, Index, pRet -> m_bInstructionFormat, bDisp, NULL, NULL, dwAddress, 
					pszOpCode, NULL, pszStringE, pszStringG, NULL, pszScaleIndex, NULL, NULL);
				pszInstruction += 5;
				dwAddress += 8;
			}
			else
			{
				PrintFormat(MOD, R_M, Base, Index, pRet -> m_bInstructionFormat, bStringCode, NULL, NULL, dwAddress, 
					pszOpCode, pszSymbol, pszStringE, pszStringG, pszBase, pszScaleIndex, NULL, NULL);
				pszInstruction += 2;
				dwAddress += 4;
			}
		}
		// Mod等于2且RM等于4
		else if (MOD == MOD_MEMORY_DWORD && R_M == SIB_FOLLOW)
		{
			// 32位偏移有SIB
			BYTE bSib = *++pszInstruction;
			BYTE Scale = GET_SCALE(bSib);
			BYTE Index = GET_INDEX(bSib);
			BYTE Base = GET_BASE(bSib);
			PCHAR pszBase = Table_Sib_Base[Base];
			PCHAR pszScaleIndex = Table_Scale_Index_Base[Scale][Index];
			PCHAR pszSymbol = "+";
			DWORD dwStringCode = *(PDWORD)(pszInstruction + 1);
			if (dwStringCode >= 0x80000000)
			{
				dwStringCode = 0x100000000 - dwStringCode;
				pszSymbol = "-";
			}
			// [scaled index] + disp32 + [EBP]
			if (Base == SIB_BASE)
			{
				DWORD dwDisp = *(LPDWORD)++pszInstruction;
				PrintFormat(MOD, R_M, Base, Index, pRet -> m_bInstructionFormat, NULL, NULL, NULL, dwAddress, 
					pszOpCode, NULL, pszStringE, pszStringG, NULL, pszScaleIndex, NULL, dwDisp);
				pszInstruction += 9;
				dwAddress += 11;
			}
			else
			{
				PrintFormat(MOD, R_M, Base, Index, pRet -> m_bInstructionFormat, NULL, NULL, dwStringCode, dwAddress, 
					pszOpCode, pszSymbol, pszStringE, pszStringG, pszBase, pszScaleIndex, NULL, NULL);
				pszInstruction += 5;
				dwAddress += 7;
			}
		}
		goto todo;
	}

	return RETURN_SUCCESS;
}