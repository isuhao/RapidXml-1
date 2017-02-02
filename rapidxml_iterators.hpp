#ifndef RAPIDXML_ITERATORS_HPP_INCLUDED
#define RAPIDXML_ITERATORS_HPP_INCLUDED

// Copyright (C) 2006, 2009 Marcin Kalicinski
// Version 1.13
// Revision $DateTime: 2009/05/15 23:02:39 $
//! \file rapidxml_iterators.hpp This file contains rapidxml iterators

#include "rapidxml.hpp"

namespace rapidxml
{

    //! Iterator of child nodes of xml_node
    template<class Ch = char>
    class node_iterator
    {

    public:
        typedef xml_node<Ch> value_type;
        typedef xml_node<Ch> &reference;
        typedef xml_node<Ch> *pointer;
        typedef std::ptrdiff_t difference_type;
        typedef std::bidirectional_iterator_tag iterator_category;

        node_iterator()
            : m_node(0)
        {
        }

        node_iterator(const xml_node<Ch> *node)
            : m_node(node->first_node())
        {
        }

        reference operator*() const
        {
            assert(m_node);
            return *m_node;
        }

        pointer operator->() const
        {
            assert(m_node);
            return m_node;
        }

        node_iterator &operator++()
        {
            assert(m_node);
            m_node = m_node->next_sibling();
            return *this;
        }

        node_iterator operator++(int)
        {
            node_iterator tmp = *this;
            this->operator++();
            return tmp;
        }

        node_iterator &operator--()
        {
            assert(m_node && m_node->previous_sibling());
            m_node = m_node->previous_sibling();
            return *this;
        }

        node_iterator operator--(int)
        {
            node_iterator tmp = *this;
            this->operator--();
            return tmp;
        }

        bool operator==(const node_iterator<Ch> &rhs) const
        {
            return m_node == rhs.m_node;
        }

        bool operator!=(const node_iterator<Ch> &rhs) const
        {
            return m_node != rhs.m_node;
        }

    private:
        xml_node<Ch> *m_node;
    };

    //! Iterator of child attributes of xml_node
    template<class Ch = char>
    class attribute_iterator
    {

    public:
        typedef xml_attribute<Ch> value_type;
        typedef xml_attribute<Ch> &reference;
        typedef xml_attribute<Ch> *pointer;
        typedef std::ptrdiff_t difference_type;
        typedef std::bidirectional_iterator_tag iterator_category;

        attribute_iterator()
            : m_attribute(0)
        {
        }

        attribute_iterator(const xml_node<Ch> *node)
            : m_attribute(node->first_attribute())
        {
        }

        reference operator*() const
        {
            assert(m_attribute);
            return *m_attribute;
        }

        pointer operator->() const
        {
            assert(m_attribute);
            return m_attribute;
        }

        attribute_iterator &operator++()
        {
            assert(m_attribute);
            m_attribute = m_attribute->next_attribute();
            return *this;
        }

        attribute_iterator operator++(int)
        {
            attribute_iterator tmp = *this;
            this->operator++();
            return tmp;
        }

        attribute_iterator &operator--()
        {
            assert(m_attribute && m_attribute->previous_attribute());
            m_attribute = m_attribute->previous_attribute();
            return *this;
        }

        attribute_iterator operator--(int)
        {
            attribute_iterator tmp = *this;
            this->operator--();
            return tmp;
        }

        bool operator==(const attribute_iterator<Ch> &rhs) const
        {
            return m_attribute == rhs.m_attribute;
        }

        bool operator!=(const attribute_iterator<Ch> &rhs) const
        {
            return m_attribute != rhs.m_attribute;
        }

    private:
        xml_attribute<Ch> *m_attribute;
    };

    //! \cond internal
    namespace internal
    {
        // Range support for children of xml_node.

        struct node_end
        {
        };

        template<class Ch>
        struct node_begin
        {

            bool operator!=(node_end) const noexcept
            {
                return m_child;
            }

            auto operator*() const noexcept
            {
                return m_child;
            }

            void operator++() noexcept
            {
                m_child = m_child->next_sibling(m_name);
            }

            xml_node<Ch> *m_child;
            std::basic_string_view<Ch> m_name;
        };

        template<class Ch>
        struct node_range
        {

            node_begin<Ch> m_begin;

            auto end()
            {
                return node_end{};
            }
            auto begin()
            {
                return m_begin;
            }
        };

        // Range support for attributes of xml_node.

        struct attribute_end
        {
        };

        template<class Ch>
        struct attribute_begin
        {

            bool operator!=(attribute_end) const noexcept
            {
                return m_attribute;
            }

            auto operator*() const noexcept
            {
                return m_attribute;
            }

            void operator++() noexcept
            {
                m_attribute = m_attribute->next_attribute(m_name);
            }

            xml_attribute<Ch> *m_attribute;
            std::basic_string_view<Ch> m_name;
        };

        template<class Ch>
        struct attribute_range
        {

            attribute_begin<Ch> m_begin;

            auto end()
            {
                return attribute_end{};
            }
            auto begin()
            {
                return m_begin;
            }
        };
    }
    //! \endcond

    //! \pre `parent` is not equal to `nullptr`.
    //! \return A range of pointers to the children of `parent`, optionally filtered by `name`.
    //! \param name Name of the children in the range. If empty, they are not filtered.
    //! \remarks The range offers minimal range-based for loop support.
    //! \remarks It is unspecified wether this function has a defaulted `name` or is overloaded.
    template<class Ch>
    auto node_range(const xml_node<Ch> *parent, std::basic_string_view<Ch> name = {}) noexcept
    {
        return internal::node_range<Ch>{{parent->first_node(), name}};
    }

    //! \pre `node` is not equal to `nullptr`.
    //! \return A range of pointers to the attributes of `node`, optionally filtered by `name`.
    //! \param name Name of the attributes in the range. If empty, they are not filtered.
    //! \remarks The range offers minimal range-based for loop support.
    //! \remarks It is unspecified wether this function has a defaulted `name` or is overloaded.
    template<class Ch>
    auto attribute_range(const xml_node<Ch> *node, std::basic_string_view<Ch> name = {}) noexcept
    {
        return internal::attribute_range<Ch>{{node->first_attribute(), name}};
    }
}

#endif
