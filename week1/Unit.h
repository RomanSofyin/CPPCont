#pragma once


struct Unit
{
	explicit Unit(size_t id)
		: id_(id)
	{}

	size_t id() const { return id_; }

private:
	size_t id_;
};

// ������� ����� ��� ��������
struct Animal : virtual Unit
{
	// name ������ �������� ���������
	// "bear" ��� �������
	// "pig" ��� ������
	Animal(std::string const & name, size_t id) : Unit(id), name_(name) {}

	std::string const& name() const { return name_; }

private:
	std::string name_;
};

// ����� ��� ��������
struct Man : virtual Unit
{
	explicit Man(size_t id) : Unit(id) {}
};

/*
 * Failed. Runtime error
 * terminate called after throwing an instance of 'std::logic_error'
 *  what():  Bear and pig are different animals
 * Aborted (core dumped)
 */

// ����� ��� �������
struct Bear : Animal
{
	explicit Bear(size_t id) : Unit(id), Animal("bear", id) {}
};

// ����� ��� ������
struct Pig : Animal
{
	explicit Pig(size_t id) : Unit(id), Animal("pig", id) {}
};

// ����� ��� ���������������
struct ManBearPig : Man, Bear, Pig
{
	ManBearPig(size_t id) : Unit(id), Bear(id), Pig(id), Man(id) {}
};