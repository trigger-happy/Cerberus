/*
Copyright (C) 2009 James Choa

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#ifndef SINGLETON_H
#define SINGLETON_H

/*!
\brief A template for implementing a Singleton using CRTP.

To use, declare a class as:
class A : public Singleton\<A\>
This will give it a getInstance() function that will return
a reference to the single class instance.
*/
template<typename T>
class Singleton
{
public:
        /*!
        Return an instance to the class
        \return A reference to the class
        */
        static T& getInstance() {
                static T inst;
                return inst;
        }
protected:
        Singleton() {
                ;
        }
};

#endif
