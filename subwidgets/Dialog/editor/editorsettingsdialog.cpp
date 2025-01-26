#include "editorsettingsdialog.h"

EditorSettingsDialog::EditorSettingsDialog(EditorConfig editorConfig, QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::EditorSettingsDialogClass())
{
	ui->setupUi(this);
	// 创建窗口列表
	m_widgets.append(ui->fontFrame);
	m_widgets.append(ui->colorFrame);

	// 创建窗口布局
	QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
	splitter->addWidget(ui->toolBox);
	splitter->addWidget(ui->settingsFrame);
	splitter->addWidget(ui->previewWidget);
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(splitter);
	mainLayout->addWidget(ui->buttonWidget);
	this->setLayout(mainLayout);

	// 其他控件设置
	QIntValidator *validator = new QIntValidator(5, 50, this);
	ui->fontSizeLineEdit->setValidator(validator);

	// 根据参数，设置初始值
	m_editorConfig = editorConfig;
	loadCurrentEditorConfig();
	
	// 应用设置按钮
	connect(ui->applyPushButton, &QPushButton::clicked, this, [=]() {
		emit applySettings(m_editorConfig);
		});
	connect(ui->confirmPushButton, &QPushButton::clicked, this, &QDialog::accept);
	connect(ui->cancelPushButton, &QPushButton::clicked, this, &QDialog::reject);
	// 连接信号和槽
	connect(ui->toolBox, &QToolBox::currentChanged, this, &EditorSettingsDialog::updateInterface);
	// 字体样式
	connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, this, [=](const QFont& f) {
		m_editorConfig.font.font = f;
		});
	// 字体大小
	connect(ui->fontSizeLineEdit, &QLineEdit::textChanged, this, [=](const QString& s) {
		m_editorConfig.font.size = s.toInt();
		});
	// 颜色选择
	connect(ui->backGroundColorPushButton, &QPushButton::clicked, this, [=]() {
		ColorPickerDialog* colorPickerDialog = new ColorPickerDialog(this);
		colorPickerDialog->exec();
		});

	// 初始界面
	ui->toolBox->setCurrentIndex(0);
	updateInterface(0);
}

EditorSettingsDialog::~EditorSettingsDialog()
{
	delete ui;
}

EditorConfig EditorSettingsDialog::getEditorConfig()
{
	return m_editorConfig;
}

// 更新界面
void EditorSettingsDialog::updateInterface(int index)
{
	// 隐藏所有窗口
	for (QWidget* widget : m_widgets)
	{
		widget->setVisible(false);
	}
	// 显示当前窗口
	if (index == 0) {
		ui->fontFrame->setVisible(true);
	}
	else if (index == 1) {
		ui->colorFrame->setVisible(true);
	}
}

void EditorSettingsDialog::loadCurrentEditorConfig()
{
	ui->fontComboBox->setCurrentFont(m_editorConfig.font.font);
	ui->fontSizeLineEdit->setText(QString::number(m_editorConfig.font.size));
}
