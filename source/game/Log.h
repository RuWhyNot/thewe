#ifndef LOG_H
#define LOG_H

#include <string>

/**
 * �����, ������� �������� �� ����� ����� �� ������ ����������
 * ����������� ����������� ��������� ���� ������
 */
class Log
{
public:
	static Log& Instance();

	void writeError(const std::string& text);
	void writeWarning(const std::string& text);
	void writeLog(const std::string& text);
	void writeInit(const std::string& text);

private:
	/** ������� ������ � ������� ������� */
	void writeLine(const std::string& text);

	std::ofstream *logFileStream;

	/** ������������ ���������� � ��������� */
	static void create();

	/** ������� ����������� ��� ������� ���������� � ��� ������������� ��������� */
	static void onDeadReference();

	/** ���������� �������� � ������������ ����� ������������ ��� */
	static void killPhoenixSingletone();

	static Log* singleInstance;

	static bool isDestroyed;
	static bool isFirstLife;

	Log();
	~Log();
	Log(const Log&);
	void operator=(const Log&);
};

#endif