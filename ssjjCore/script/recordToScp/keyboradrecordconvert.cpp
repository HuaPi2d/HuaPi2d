#include "keyboradrecordconvert.h"

QString convertRecordToScp(QString record)
{
	QString scp;
	scp += "# 录制开始\n";
    QRegularExpression pattern("\\s?(?<action_type>\\w+):(?<pos_x>\\d+):(?<pos_y>\\d+):(?<key_code>\\d+)\\s*SLEEP:(?<sleep_time>\\d+)");
    QRegularExpressionMatchIterator matchIterator = pattern.globalMatch(record);
    QMap<int, KeyEvent> keyMap;
    QRegularExpressionMatch match;
    // 点击型按键
    enum ClickTypeKey{C = 0, E, Q, X, NUM1, NUM2, NUM3, NUM4, NUM5, KEY_NUM};
    int ClickTypeKeyCode[KEY_NUM] = {67, 69, 81, 88, 49, 50, 51, 52, 53};
    QString ClickTypeKeyStr[KEY_NUM] = {"C", "E", "Q", "X", "1", "2", "3", "4", "5"};
    bool isShooting = false;
	int flag_num = 0;
    int total_time = 0;
    while (matchIterator.hasNext()) {
        match = matchIterator.next();
        QString action_type = match.captured("action_type");
        int pos_x = match.captured("pos_x").toInt();
        int pos_y = match.captured("pos_y").toInt();
        int key_code = match.captured("key_code").toInt();
        int sleep_time = match.captured("sleep_time").toInt();

        // 人物移动
		if (key_code == 87) {
			if (action_type == "KEYDOWN") {
				if (keyMap[87].type != "KEYDOWN") {
					keyMap[87].type = "KEYDOWN";
					scp += QString("sleep_ms(%1)\n").arg(total_time);
					total_time = 0;
					scp += QString("key_down(\"W\")\n");
				}
			}
			else if (action_type == "KEYUP") {
				keyMap[87].type = "KEYUP";
				scp += QString("sleep_ms(%1)\n").arg(total_time);
				total_time = 0;
				scp += QString("key_up(\"W\")\n");
			}
		}
		else if (key_code == 65) {
			if (action_type == "KEYDOWN") {
				if (keyMap[65].type != "KEYDOWN") {
					keyMap[65].type = "KEYDOWN";
					scp += QString("sleep_ms(%1)\n").arg(total_time);
					total_time = 0;
					scp += QString("key_down(\"A\")\n");
				}
			}
			else if (action_type == "KEYUP") {
				keyMap[65].type = "KEYUP";
				scp += QString("sleep_ms(%1)\n").arg(total_time);
				total_time = 0;
				scp += QString("key_up(\"A\")\n");
			}
		}
		else if (key_code == 68) {
			if (action_type == "KEYDOWN") {
				if (keyMap[68].type != "KEYDOWN") {
					keyMap[68].type = "KEYDOWN";
					scp += QString("sleep_ms(%1)\n").arg(total_time);
					total_time = 0;
					scp += QString("key_down(\"D\")\n");
				}
			}
			else if (action_type == "KEYUP") {
				keyMap[68].type = "KEYUP";
				scp += QString("sleep_ms(%1)\n").arg(total_time);
				total_time = 0;
				scp += QString("key_up(\"D\")\n");
			}
		}
		else if (key_code == 83) {
			if (action_type == "KEYDOWN") {
				if (keyMap[83].type != "KEYDOWN") {
					keyMap[83].type = "KEYDOWN";
					scp += QString("sleep_ms(%1)\n").arg(total_time);
					total_time = 0;
					scp += QString("key_down(\"S\")\n");
				}
			}
			else if (action_type == "KEYUP") {
				keyMap[83].type = "KEYUP";
				scp += QString("sleep_ms(%1)\n").arg(total_time);
				total_time = 0;
				scp += QString("key_up(\"S\")\n");
			}
		}
		// "R"
		else if (key_code == 82) {
			if (action_type == "KEYDOWN") {
				if (keyMap[82].type != "KEYDOWN") {
					keyMap[82].type = "KEYDOWN";
					scp += QString("sleep_ms(%1)\n").arg(total_time);
					total_time = 0;
					scp += QString("key_down(\"R\")\n");
				}
			}
			else if (action_type == "KEYUP") {
				keyMap[82].type = "KEYUP";
				scp += QString("sleep_ms(%1)\n").arg(total_time);
				total_time = 0;
				scp += QString("key_up(\"R\")\n");
			}
		}
		// 鼠标
		else if (key_code == 39) {
			if (action_type == "KEYDOWN") {
				if (keyMap[39].type != "KEYDOWN") {
					keyMap[39].type = "KEYDOWN";
					scp += QString("sleep_ms(%1)\n").arg(total_time);
					total_time = 0;
					scp += QString("turn_around(-45)\n");
				}
			}
			else if (action_type == "KEYUP") {
				keyMap[39].type = "KEYUP";
			}
		}
		else if (key_code == 37) {
			if (action_type == "KEYDOWN") {
				if (keyMap[37].type != "KEYDOWN") {
					keyMap[37].type = "KEYDOWN";
					scp += QString("sleep_ms(%1)\n").arg(total_time);
					total_time = 0;
					scp += QString("turn_around(45)\n");
				}
			}
			else if (action_type == "KEYUP") {
				keyMap[37].type = "KEYUP";
			}
		}
		else if (key_code == 40) {
			if (action_type == "KEYDOWN") {
				if (keyMap[40].type != "KEYDOWN") {
					keyMap[40].type = "KEYDOWN";
					scp += QString("sleep_ms(%1)\n").arg(total_time);
					total_time = 0;
					scp += QString("turn_up_down(-30)\n");
				}
			}
			else if (action_type == "KEYUP") {
				keyMap[40].type = "KEYUP";
			}
		}
		else if (key_code == 38) {
			if (action_type == "KEYDOWN") {
				if (keyMap[38].type != "KEYDOWN") {
					keyMap[38].type = "KEYDOWN";
					scp += QString("sleep_ms(%1)\n").arg(total_time);
					total_time = 0;
					scp += QString("turn_up_down(30)\n");
				}
			}
			else if (action_type == "KEYUP") {
				keyMap[38].type = "KEYUP";
			}
		}
		else if (key_code == 191) {
			if (action_type == "KEYDOWN") {
				if (keyMap[191].type != "KEYDOWN") {
					keyMap[191].type = "KEYDOWN";
					scp += QString("sleep_ms(%1)\n").arg(total_time);
					total_time = 0;
					scp += QString("middle_click()\n");
				}
			}
			else if (action_type == "KEYUP") {
				keyMap[191].type = "KEYUP";
			}
		}
		// "L"
		else if (key_code == 76) {
			if (action_type == "KEYDOWN") {
				if (keyMap[76].type != "KEYDOWN") {
					keyMap[76].type = "KEYDOWN";
					scp += QString("sleep_ms(%1)\n").arg(total_time);
					total_time = 0;
					scp += QString("# Flag Point %1\n").arg(flag_num);
				}
			}
			else if (action_type == "KEYUP") {
				keyMap[76].type = "KEYUP";
			}
		}
		// 射击
		else if (key_code == 13) {
			if (action_type == "KEYDOWN") {
				if (keyMap[13].type != "KEYDOWN") {
					keyMap[13].type = "KEYDOWN";
					if (isShooting) {
						scp += QString("sleep_ms(%1)\n").arg(total_time);
						total_time = 0;
						scp += QString("left_up()\n");
						isShooting = false;
					}
					else {
						scp += QString("sleep_ms(%1)\n").arg(total_time);
						total_time = 0;
						scp += QString("left_down()\n");
						isShooting = true;
					}
				}
			}
			else if (action_type == "KEYUP") {
				keyMap[13].type = "KEYUP";
			}
		}
		else {
			for (int i = 0; i < KEY_NUM; i++) {
				if (key_code == ClickTypeKeyCode[i]) {
					if (action_type == "KEYDOWN") {
						if (keyMap[ClickTypeKeyCode[i]].type != "KEYDOWN") {
							keyMap[ClickTypeKeyCode[i]].type = "KEYDOWN";
							scp += QString("sleep_ms(%1)\n").arg(total_time);
							total_time = 0;
							scp += QString("key_press(\"%1\")\n").arg(ClickTypeKeyStr[i]);
						}
					}
					else if (action_type == "KEYUP") {
						keyMap[ClickTypeKeyCode[i]].type = "KEYUP";
					}
				}
			}
		}
        
        total_time += sleep_time;
    }
    scp += "# 录制结束";
    return scp;
}
