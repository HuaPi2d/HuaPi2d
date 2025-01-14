#pragma once

#include <Qsci/qscilexercustom.h>
#include <Qsci/qsciscintilla.h>

class ScpLanguageLexer  : public QsciLexerCustom
{
public:
	ScpLanguageLexer(QObject *parent);
	~ScpLanguageLexer();

protected:
	// ���شʷ�����������������
	const char *language() const override;
	// ����֧�ֵ���ʽ����
	int styleBitsNeeded() const override;
	// ����ÿ����ʽ������
	QString description(int style) const override;
	// �����﷨�������߼�
	void styleText(int start, int end) override;

private:
	// ���ַ������趨��ʽ
	void setupStyles();

	// ��ʽ����ö��
	enum {
		Default = 0,
		Keyword,   // �ؼ���
		Number,    // ����
		String,    // �ַ���
		Operator,  // �����
		Function,  // ����
		Variable,  // ����
		Comment,   // ע��
	};

	QStringList keywords;  // �ؼ����б�
};
