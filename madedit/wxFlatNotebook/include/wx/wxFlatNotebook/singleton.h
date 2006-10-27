#ifndef CODELITE_SINGLETON_H
#define CODELITE_SINGLETON_H

/**
 * A template class that implements the Singleton pattern.
 *
 * \date 08-23-2006
 * \author eran
 */
template <typename T>
class Singleton
{
	static T* ms_instance;
public:
	/**
	 * Static method to access the only pointer of this instance.
	 * \return a pointer to the only instance of this 
	 */
	static T* Get();

	/**
	 * Release resources.
	 */
	static void Free();

protected:
	/**
	 * Default constructor.
	 */
	Singleton();

	/**
	 * Destructor.
	 */
	virtual ~Singleton();
};
template <typename T>
T* Singleton<T>::ms_instance = 0;

template <typename T>
Singleton<T>::Singleton()
{
}

template <typename T>
Singleton<T>::~Singleton()
{
}

template <typename T>
T* Singleton<T>::Get()
{
	if(!ms_instance)
		ms_instance = new T();
	return ms_instance;
}

template <typename T>
void Singleton<T>::Free()
{
	if( ms_instance )
	{
		delete ms_instance;
		ms_instance = 0;
	}
}

#endif // CODELITE_SINGLETON_H
