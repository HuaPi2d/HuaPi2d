#include "stdafx.h"

Recorder::Recorder(MacroCompiler *compiler, std::string filename) {
	wait = false;
	active = false;
	m_filename = filename;
	this->compiler = compiler;
	this->lastAction = 0;
}

Recorder::~Recorder() {
	for (auto act : actionList)
		delete act;
}

void Recorder::record() {
	registerRecorder(this);

	qDebug() << "等待开始录制...";
	while (wait)
		Sleep(1);
	qDebug() << "开始录制...";
	active = true;
	wait = true;
	while (wait)
		Sleep(1);
	qDebug() << "录制结束...";
	active = false;
	if (!m_filename.empty()) {
		std::vector<std::string> macroAsText;
		if (!compiler->decompileMacro(this->actionList, macroAsText)) {
			std::cout << "There was an error while decompiling macro!" << std::endl;
			return;
		}
		std::ofstream os;
		os.open(m_filename, std::ofstream::binary);
		if (os.is_open()) {
			for (std::string line:macroAsText) {
				os << line << "\n";
			}
		}
		os.close();
		qDebug() << "写入结束...";
	}
	else {
		std::cout << "No filename specified for macro recording!" << std::endl;
	}
}

void Recorder::addAction(unsigned int action, unsigned char type, DWORD key) {
	DWORD ticks = GetTickCount();
	if (!actionList.empty()) {
		// Push sleep action
		Action *act = new Action;
		act->action = 0;
		act->type = TYPE_SLEEP;
		act->key = ticks - lastAction;
		act->x = 0;
		act->y = 0;
		actionList.push_back(act);
	}
	POINT loc;
	if (GetCursorPos(&loc)) {
		Action *act = new Action;
		act->action = action;
		act->type = type;
		act->key = key;
		act->x = loc.x;
		act->y = loc.y;
		actionList.push_back(act);
	}
	lastAction = ticks;
}

void Recorder::mouseEvent(unsigned char action) {
	if (!active)
		return;
	addAction(action, TYPE_MOUSE, 0x00);
}

void Recorder::stopRecording()
{
	wait = false;
}

void Recorder::keyboardEvent(unsigned int action, DWORD key) {
	if (key == VK_F1) {
		qDebug() << "录制结束...";
		wait = false;
	}
	if (!active)
		return;
	addAction(action, TYPE_KEYBOARD, key);
}