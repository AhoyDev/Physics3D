#ifndef __GUIWINDOW_H__
#define __GUIWINDOW_H__

class GUI_Window
{
public:
	GUI_Window(const bool active = false) : active(active) {}
	~GUI_Window() {}

	virtual void Draw() {}

	void DrawWindow() { if (active) Draw(); }
	void ToggleActive() { active = !active; }
	void SetActive(const bool new_state) { active = new_state; }
	const bool IsWindowActive()const { return active; }

private:
	
	bool active;
};

#endif