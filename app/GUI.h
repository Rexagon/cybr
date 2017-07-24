#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>

#include "Layout.h"
#include "VerticalLayout.h"
#include "HorizontalLayout.h"

#include "Widget.h"
#include "Label.h"

class GUI
{
public:
	enum Alignment
	{
		AlignLeft = 1 << 0,
		AlignRight = 1 << 1,
		AlignHCenter = 1 << 2,

		AlignTop = 1 << 3,
		AlignBottom = 1 << 4,
		AlignVCenter = 1 << 5,

		AlignCenter = AlignHCenter | AlignVCenter
	};

	GUI();

	// ��������� ��������� ���������, �������, ���������
	void update();

	// ������ �������� � �������� �������
	void draw();

	// ���������� �������� ������ ����� ����
	widget_ptr getRootWidget() const { return m_rootWidget; }

    void prepareDeleting(Widget* widget);

	// ��������������� ������� ��� �������� GUI ��������
	template<class T, class... Args>
	static std::shared_ptr<T> create(const Args&... args)
	{
		static_assert(std::is_base_of<Widget, T>::value, "GUI::Create template argument must be the child class of \"Widget\" class");
		std::shared_ptr<T> widget = std::shared_ptr<T>(new T(args...));
		return std::move(widget);
	}
private:
	widget_ptr m_rootWidget;

	Widget* m_currentHoveredItem;
	Widget* m_currentPressedItem;
	Widget* m_currentFocusedItem;
};
