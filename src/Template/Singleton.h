#ifndef SINGLETON_H
#define SINGLETON_H

template<class T>
class Singleton
{
public:
	static T& Get();

private:
	class SingletonAutoRelease final
	{
	public:
		~SingletonAutoRelease()
		{
			delete SingletonInstance;
			SingletonInstance = nullptr;
		};	
	};

	static T* SingletonInstance;
};

template<class T>
T* Singleton<T>::SingletonInstance = nullptr;

template<class T>
inline T& Singleton<T>::Get()
{
	if(!SingletonInstance)
	{
		static SingletonAutoRelease GC;
		SingletonInstance = new T();
	}
	return *SingletonInstance;
}

#endif
