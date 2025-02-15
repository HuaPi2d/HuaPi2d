#include "submdwidget.h"

SubMDWidget::SubMDWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::SubMDWidgetClass())
{
	ui->setupUi(this);

	connect(ui->closePushButton, &QPushButton::clicked, this, [=]() {
		emit widgetClosed();
		});

	/* º”‘ÿ≈‰÷√ */
	loadSettings();

	// ¥¥Ω®MD±‡º≠∆˜
	mdFilesEditor = new MDFilesEditor(this);
	mdFilesEditor->m_signalsHelper->resetStyles(mdEditorConfig);

	// …Ë÷√MD±‡º≠∆˜≤ºæ÷
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(mdFilesEditor);
	ui->mdFilesEditorFrame->setLayout(layout);

	// ∂¡»°Œƒº˛ƒ⁄»›÷¡±‡º≠∆˜
	mdFilesEditor->receiveFileInfo(currentFile);
}

SubMDWidget::~SubMDWidget()
{
	/* ±£¥Ê≈‰÷√ */
	saveSettings();
	delete ui;
}

void SubMDWidget::loadSettings()
{
	UsersSettings settings(qApp->applicationDirPath() + "/Settings/mdFilesSettings.ini");

	/* ∂¡»°≈‰÷√ */
	settings.beginGroup("MDFiles");
	if (settings.value("currentFile") != "") 
		currentFile = settings.value("currentFile", ":/default/resources/default/Default.md").toString();
	else
		currentFile = ":/default/resources/default/Default.md";
	settings.endGroup();

	// »´æ÷±‡º≠∆˜≈‰÷√
	/* …˘√˜∂‘œÛ */
	UsersSettings editorConfigSettings(qApp->applicationDirPath() + "/Settings/editorConfig.ini");

	/* ∂¡»°±‡º≠∆˜≈‰÷√ */
	//// ±‡º≠∆˜≈‰÷√
	editorConfigSettings.beginGroup("mdEditorConfig");
	mdEditorConfig.font.font = editorConfigSettings.value("font", QFont("Consolas")).value<QFont>();
	mdEditorConfig.font.size = editorConfigSettings.value("fontSize", 12).toInt();
	editorConfigSettings.beginGroup("color");
	mdEditorConfig.color.background = QColor(editorConfigSettings.value("background", "#2b2b2b").toString());
	mdEditorConfig.color.foreground = QColor(editorConfigSettings.value("foreground", "#FAFAFA").toString());
	mdEditorConfig.color.text = QColor(editorConfigSettings.value("text", "#FFFFFF").toString());
	mdEditorConfig.color.keyword = QColor(editorConfigSettings.value("keyword", "#6A7FCC").toString());
	mdEditorConfig.color.number = QColor(editorConfigSettings.value("number", "#1793D0").toString());
	mdEditorConfig.color.string = QColor(editorConfigSettings.value("string", "#8CD000").toString());
	mdEditorConfig.color.operateur = QColor(editorConfigSettings.value("operateur", "#FF8F00").toString());
	mdEditorConfig.color.function = QColor(editorConfigSettings.value("function", "#FF3DB5").toString());
	mdEditorConfig.color.variable = QColor(editorConfigSettings.value("variable", "#000099").toString());
	mdEditorConfig.color.comment = QColor(editorConfigSettings.value("comment", "#999999").toString());
	mdEditorConfig.color.caret_line_color = QColor(editorConfigSettings.value("caret_line_color", "#323232").toString());
	mdEditorConfig.margin.line_number_color = QColor(editorConfigSettings.value("line_number_color", "#8a8a8a").toString());
	mdEditorConfig.margin.background_color = QColor(editorConfigSettings.value("background_color", "#313335").toString());
	editorConfigSettings.endGroup();
	editorConfigSettings.endGroup();
}

void SubMDWidget::saveSettings()
{
	UsersSettings settings(qApp->applicationDirPath() + "/Settings/mdFilesSettings.ini");

	/* –¥»Î≈‰÷√ */
	settings.beginGroup("MDFiles");
	settings.setValue("currentFile", currentFile);
	settings.endGroup();


	/* …˘√˜∂‘œÛ */
	UsersSettings editorConfigSettings(qApp->applicationDirPath() + "/Settings/editorConfig.ini");

	/* –¥»Î±‡º≠∆˜≈‰÷√ */
	editorConfigSettings.beginGroup("mdEditorConfig");
	editorConfigSettings.setValue("font", mdEditorConfig.font.font.toString());
	editorConfigSettings.setValue("fontSize", mdEditorConfig.font.size);
	editorConfigSettings.beginGroup("color");
	editorConfigSettings.setValue("background", mdEditorConfig.color.background.name());
	editorConfigSettings.setValue("text", mdEditorConfig.color.text.name());
	editorConfigSettings.setValue("keyword", mdEditorConfig.color.keyword.name());
	editorConfigSettings.setValue("number", mdEditorConfig.color.number.name());
	editorConfigSettings.setValue("string", mdEditorConfig.color.string.name());
	editorConfigSettings.setValue("operateur", mdEditorConfig.color.operateur.name());
	editorConfigSettings.setValue("function", mdEditorConfig.color.function.name());
	editorConfigSettings.setValue("variable", mdEditorConfig.color.variable.name());
	editorConfigSettings.setValue("comment", mdEditorConfig.color.comment.name());
	editorConfigSettings.setValue("caret_line_color", mdEditorConfig.color.caret_line_color.name());
	editorConfigSettings.setValue("line_number_color", mdEditorConfig.margin.line_number_color.name());
	editorConfigSettings.setValue("background_color", mdEditorConfig.margin.background_color.name());
	editorConfigSettings.endGroup();
	editorConfigSettings.endGroup();
}
