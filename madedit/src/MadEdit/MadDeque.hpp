///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadDeque.hpp
// Description: a simple but fast deque implementation
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include <cstddef>

template <typename T>
class MadDeque
{
public:
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef size_t size_type;

private:
    struct buffer
    {
        static const size_type N = 8192;
        MadDeque<T> *mdeque;
        T* begin;
        T* end;
        buffer* prev;
        buffer* next;
        buffer(MadDeque<T> *d) :
            mdeque(d),
            begin((T*)new char[N * sizeof(T)]),
            end(begin + N),
            prev(NULL),
            next(NULL)
        {
        }
        ~buffer()
        {
            delete[] (char*)begin;
        }
    };

public:
    struct iterator
    {
        typedef typename MadDeque::value_type value_type;
        typedef typename MadDeque::reference reference;
        typedef typename MadDeque::const_reference const_reference;
        typedef typename MadDeque::pointer pointer;
        typedef typename MadDeque::size_type size_type;
        typedef size_type difference_type;
        typedef std::random_access_iterator_tag iterator_category;

        buffer *buf;
        T* ptr;

        iterator() : buf(NULL), ptr(NULL) {}
        iterator(buffer *b, T* p) : buf(b), ptr(p) {}
        iterator(const iterator& it) : buf(it.buf), ptr(it.ptr) {}

        reference operator*()
        {
            return *ptr;
        }
        const_reference operator*() const
        {
            return *ptr;
        }
        pointer operator->()
        {
            return ptr;
        }
        bool operator==(const iterator& it) const
        {
            return (ptr == it.ptr) && (buf == it.buf);
        }
        bool operator!=(const iterator& it) const
        {
            return (ptr != it.ptr) || (buf != it.buf);
        }

        iterator& operator+=(difference_type off)
        {
            if(off != 0)
            {
                do
                {
                    ++(*this);
                }
                while(--off > 0);
            }
            return *this;
        }
        iterator& operator-=(difference_type off)
        {
            if(off != 0)
            {
                do
                {
                    --(*this);
                }
                while(--off > 0);
            }
            return *this;
        }

        // pre-increment operator
        iterator& operator++()
        {
            if(++ptr == buf->end)
            {
                if(buf->next == NULL)
                {
                    //assert(buf==buf->mdeque->m_last_buffrt);
                    buf->mdeque->add_buffer_back();
                }
                buf = buf->next;
                ptr = buf->begin;
            }
            return *this;
        }
        // post-increment operator
        iterator operator++(int)
        {
            iterator it = *this;
            ++(*this);
            return it;
        }
        // pre-decrement operator
        iterator& operator--()
        {
            if(ptr == buf->begin)
            {
                buf = buf->prev;
                ptr = buf->end;
            }
            --ptr;
            return *this;
        }
        // post-decrement operator
        iterator operator--(int)
        {
            iterator it = *this;
            --(*this);
            return it;
        }
    };

private:
    size_type m_size;
    buffer *m_first_buffer, *m_last_buffer;
    iterator m_begin_iterator, m_end_iterator;

private:
    // buffer methods
    void add_buffer_back()
    {
        buffer *tmp = new buffer(this);
        tmp->prev = m_last_buffer;
        m_last_buffer->next = tmp;
        m_last_buffer = tmp;
    }
    void delete_first_buffer()
    {
        if(m_first_buffer != m_last_buffer)
        {
            buffer *tmp = m_first_buffer->next;
            delete m_first_buffer;
            m_first_buffer = tmp;
            m_first_buffer->prev = NULL;
        }
    }
    void free_buffer()
    {
        buffer *tmp;
        do
        {
            tmp = m_first_buffer->next;
            delete m_first_buffer;
            m_first_buffer = tmp;
        }while(m_first_buffer != NULL);
        m_last_buffer = NULL;
    }
    pointer get_pointer(size_type index) const
    {
        //assert(index<m_size)
        if(index <= m_size/2) // from begin
        {
            buffer *buf = m_begin_iterator.buf;
            T* ptr = m_begin_iterator.ptr;
            size_type count = buf->end - ptr;
            while(index >= count)
            {
                buf = buf->next;
                ptr = buf->begin;
                index -= count;
                count = buffer::N;
            }
            return ptr + index;
        }

        // from end
        index = m_size - index;
        buffer *buf = m_end_iterator.buf;
        T* ptr = buf->begin;
        if(buf == m_begin_iterator.buf)
        {
            ptr = m_begin_iterator.ptr;
        }
        size_type count = m_end_iterator.ptr - ptr;
        while(index > count)
        {
            buf = buf->prev;
            ptr = buf->begin;
            if(buf == m_begin_iterator.buf)
            {
                ptr = m_begin_iterator.ptr;
            }
            index -= count;
            count = buf->end - ptr;
        }
        return ptr + (count - index);
    }

public:
    MadDeque() :
        m_size(0),
        m_first_buffer(new buffer(this)),
        m_last_buffer(m_first_buffer),
        m_begin_iterator(iterator(m_first_buffer, m_first_buffer->begin)),
        m_end_iterator(m_begin_iterator)
    {
    }
    MadDeque(const MadDeque& d) :
        m_size(0),
        m_first_buffer(new buffer(this)),
        m_last_buffer(m_first_buffer),
        m_begin_iterator(iterator(m_first_buffer, m_first_buffer->begin)),
        m_end_iterator(m_begin_iterator)
    {
        this->operator=(d);
    }
    ~MadDeque()
    {
        while(m_begin_iterator != m_end_iterator)
        {
            (m_begin_iterator.ptr)->~T();
            ++m_begin_iterator;
        }
        free_buffer();
    }

    MadDeque& operator=(const MadDeque& d)
    {
        clear(); // destroy old T

        if(!d.empty())
        {
            iterator it = d.begin(), itend = d.end();
            do
            {
                this->push_back(*it);
            }
            while(++it != itend);
        }

        return *this;
    }

    bool operator==(const MadDeque& d) const
    {
        if(m_size != d.m_size) return false;
        if(m_size == 0) return true;

        iterator it0 = begin(), it0end = end();
        iterator it1 = d.begin();
        while(*it0 == *it1)
        {
            ++it1;
            if(++it0 == it0end) return true;
        }
        return false;
    }
    bool operator!=(const MadDeque& d) const
    {
        return !this->operator==(d);
    }

    iterator begin() const
    {
        return m_begin_iterator;
    }
    iterator end() const
    {
        return m_end_iterator;
    }
    size_type size() const
    {
        return m_size;
    }
    bool empty() const
    {
        return m_size == 0;
    }
    reference operator[](size_type n)
    {
        return *get_pointer(n);
    }
    const_reference operator[](size_type n) const
    {
        return *get_pointer(n);
    }
    reference front()
    {
        return *m_begin_iterator;
    }
    reference back()
    {
        iterator it = m_end_iterator;
        return *--it;
    }
    void push_back(const T& x)
    {
        new(m_end_iterator.ptr) T(x);
        ++m_end_iterator;
        ++m_size;
    }
    void pop_front()
    {
        (m_begin_iterator.ptr)->~T();
        ++m_begin_iterator;
        --m_size;
        if(m_begin_iterator.buf != m_first_buffer)
        {
            delete_first_buffer();
        }
    }
    void pop_back()
    {
        --m_end_iterator;
        (m_end_iterator.ptr)->~T();
        --m_size;
    }
    void clear()
    {
        while(m_begin_iterator != m_end_iterator)
        {
            (m_begin_iterator.ptr)->~T();
            ++m_begin_iterator;
        }
        m_size = 0;
        m_begin_iterator.buf = m_first_buffer;
        m_begin_iterator.ptr = m_first_buffer->begin;
        m_end_iterator = m_begin_iterator;
    }
};
