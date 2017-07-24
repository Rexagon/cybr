#pragma once

#include <vector>

#include "Widget.h"

class Layout
{
public:
	// ������ �� ����������� �� � ���� ������
	Layout();

	// ������ ������ � ����������� ��� � parent
	Layout(widget_ptr parent);

	virtual ~Layout();

	// ��������� ������ � ��������� ������
	virtual void addWidget(widget_ptr widget);

	// ������� ������ � ��������� ������
	virtual void removeWidget(widget_ptr widget);

	// ���������� ����� ������� ������ �������
	//@ ���� ������ �� �����, ����� -1
	virtual int indexOf(widget_ptr widget) const;

	// ���������� ������ ��� ��������� �������
	//@ ���� ����� �� �����, ����� nullptr
	virtual Widget* takeAt(int index) const;

    void clear();

	// ������������� ������������ ��� ���� ��������� ������� � ��������� ������
	//@ ���� �� �������� ������������ �� �����-������ �� ����
	//@ �� ��� ���� ��� �� ��������� ����� ������������ �� ������
	void setAlignment(int alignment);

	// ������������� ������������ ��� ���������� �������� � ��������� ������
	//@ ���� ������ �������� ���, �� ������ �� ���������
	//@ ���� �� �������� ������������ �� �����-������ �� ����
	//@ �� ��� ���� ��� �� ��������� ����� ������������ �� ������
	void setAlignment(widget_ptr widget, int alignment);

	// ������������� ������� ����� ���������� � ������� �� ���� �������
	void setSpacing(float spacing) { m_spacing = spacing; }

	// ���������� ������ ������� ����� ���������� � �� ���� �������
	float getSpacing() const { return m_spacing; }
protected:
	friend class GUI;
	friend class Widget;

	virtual void update();

	Widget* m_parent;

	float m_spacing;

	struct Item
	{
		Item(Widget* widget, int alignment) :
			widget(widget), alignment(alignment) {}

		Widget* widget;
		int alignment;
	};
	std::vector<Item> m_items;
	std::vector<widget_ptr> m_orderedWidgets;
};
