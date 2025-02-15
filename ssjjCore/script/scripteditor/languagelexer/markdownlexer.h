#pragma once

#include <Qsci/qscilexercustom.h>
#include <QRegularExpression>
#include <Qsci/qsciscintilla.h>
#include <bitset>

#include "struct/editor/EditorConfig.h"


class MarkdownLexer  : public QsciLexerCustom
{
public:
	MarkdownLexer(QObject *parent);
	~MarkdownLexer();

public slots:
	void resetStyles(EditorConfig config);

protected:
	// ���شʷ�����������������
	const char *language() const override;
	// ����֧�ֵ���ʽ����
	int styleBitsNeeded() const override;
	// ����ÿ����ʽ������
	QString description(int style) const override;
	// �����﷨�����߼�
	void styleText(int start, int end) override;

private:
	EditorConfig m_editorConfig;

	enum {
		Default = 0, // Ĭ����ʽ
		Header1, Header2, Header3, Header4, Header5, Header6, // ����
		Monospace, // �ȿ�����
		StrikeMono, // ɾ��������
		List, // �б�
		Ruler, // �ָ���
		Bold, Italic, Strike, BoldItalic, StrikeBold, StrikeItalic, StrikeBoldItalic,
		Quote, // ����
		BoldQuote, ItalicQuote, StrikeQuote, BoldItalicQuote, StrikeBoldQuote, StrikeItalicQuote, StrikeBoldItalicQuote, 
		Brackets, // ������
		Brace = Quote, // ������
		Tag = Brackets // ��ǩ
	};
};
