#pragma once
#include "stdafx.h"
class ProcessInfo
{
public:
	ProcessInfo()
	{
	}
	ProcessInfo(CString imageName,
	DWORD pid,
	DWORD threadNums,
	INT cpu,
	uint64_t memory,
	CString des) :m_imageName(imageName), m_pid(pid),
		m_threadNums(threadNums), m_cpu(cpu), m_memory(memory)
		, m_des(des)
	{
	}
	~ProcessInfo()
	{
	}
	CString& getImageName() {
		return m_imageName;
	}
	DWORD getPid() const{
		return m_pid;
	}
	DWORD getThreadNums() {
		return m_threadNums;
	}
	INT getCpu() {
		return m_cpu;
	}
	uint64_t getMemory() {
		return m_memory;
	}
	CString& getDes() {
		return m_des;
	}
	bool operator()(ProcessInfo *info1,ProcessInfo *info2) {
		return info1->m_pid < info2->m_pid;
	}
private:
	CString m_imageName;
	DWORD m_pid;
	DWORD m_threadNums;
	INT m_cpu;
	uint64_t m_memory;
	CString m_des;
};