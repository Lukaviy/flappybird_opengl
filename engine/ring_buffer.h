#pragma once
#include <iterator>
#include <vector>

class RingBufferException_t {};
class BufferOverflowException_t : public RingBufferException_t {};
class BufferEmptyException_t : public RingBufferException_t {};

template<class T>
class RingBuffer_t {
	template<class TType, class BuffRefType>
	class basic_iterator : public std::iterator<std::random_access_iterator_tag, T> {
	public:
		basic_iterator() : _buffer(nullptr), _element(0) {}
		basic_iterator& operator++() { ++_element; return *this; }
		basic_iterator operator++(int) { basic_iterator tmp(*this); operator++(); return tmp; }
		basic_iterator& operator--() { --_element; return *this; }
		basic_iterator operator--(int) { basic_iterator tmp(*this); operator--(); return tmp; }
		basic_iterator operator=(const basic_iterator& it) { _buffer = it._buffer; _element = it._element; return *this; }
		basic_iterator& operator+=(const int i) { _element += i; return *this; }
		basic_iterator& operator-=(const int i) { _element -= i; return *this; }
		basic_iterator& operator-(const int i) { auto t = *this; t -= i; return *this; }
		basic_iterator& operator+(const int i) { auto t = *this; t += i; return *this; }
		bool operator==(const basic_iterator& rhs) const { return _element == rhs._element; }
		bool operator!=(const basic_iterator& rhs) const { return _element != rhs._element; }
		TType& operator*() { return _buffer->at(_element); }
		TType* operator->() { return &(_buffer->at(_element)); }
		operator size_t() const { return _element; }
	protected:
		friend RingBuffer_t;
		basic_iterator(BuffRefType buffer, size_t element) : _buffer(buffer), _element(element) {}
		BuffRefType _buffer;
		size_t _element;
	};
public:
	RingBuffer_t(size_t size);
	void push_back(const T& val);
	void push_back_override(const T& val);
	void pop_front();
	
	T& front();
	T& back();

	void clear();

	size_t size();

	using iterator = basic_iterator<T, RingBuffer_t*>;
	using const_iterator = basic_iterator<const T, const RingBuffer_t*>;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
private:
	std::vector<T> _data;
	size_t _head;
	size_t _tail;

	T& at(size_t i);
	const T& at(size_t i) const;
};

template <class T>
RingBuffer_t<T>::RingBuffer_t(size_t size) : _data(size), _head(0), _tail(0) {}

template <class T>
typename RingBuffer_t<T>::iterator RingBuffer_t<T>::begin() {
	return iterator(this, _head);
}

template <class T>
typename RingBuffer_t<T>::iterator RingBuffer_t<T>::end() {
	return iterator(this, _tail);
}

template <class T>
typename RingBuffer_t<T>::const_iterator RingBuffer_t<T>::begin() const {
	return const_iterator(this, _head);
}

template <class T>
typename RingBuffer_t<T>::const_iterator RingBuffer_t<T>::end() const {
	return const_iterator(this, _tail);
}

template <class T>
T& RingBuffer_t<T>::at(size_t i) {
	return _data[i % _data.size()];
}

template <class T>
const T& RingBuffer_t<T>::at(size_t i) const {
	return _data[i % _data.size()];
}

template <class T>
void RingBuffer_t<T>::push_back(const T& val) {
	if (_tail - _head >= _data.size()) {
		throw BufferOverflowException_t();
	}
	at(_tail++) = val;
}

template <class T>
void RingBuffer_t<T>::push_back_override(const T& val) {
	if (_tail - _head >= _data.size()) {
		_head = _tail - (_data.size() - 1);
	}
	at(_tail++) = val;
}

template <class T>
void RingBuffer_t<T>::pop_front() {
	if (_tail == _head) {
		throw BufferEmptyException_t();
	}
	++_head;
}

template <class T>
T& RingBuffer_t<T>::front() {
	if (_tail == _head) {
		throw BufferEmptyException_t();
	}
	return at(_head);
}

template <class T>
T& RingBuffer_t<T>::back() {
	if (_tail == _head) {
		throw BufferEmptyException_t();
	}
	return at(_tail - 1);
}

template <class T>
void RingBuffer_t<T>::clear() {
	_head = _tail = 0;
}

template <class T>
size_t RingBuffer_t<T>::size() {
	return _data.size();
}
