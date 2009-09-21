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
class Singleton{
public:
	/*!
	Return an instance to the class
	\return A reference to the class
	*/
	static T& getInstance(){
		static T inst;
		return inst;
	}
protected:
	Singleton(){;}
};

#endif