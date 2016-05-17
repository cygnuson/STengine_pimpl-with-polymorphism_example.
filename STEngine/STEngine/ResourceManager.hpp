#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <string>
#include <map>
#include <utility>

#include "logger.hpp"

/**Manage resource in a singleton manner.
\tparam KeyType the type of key to use. Must be default constructible.
\tparam ValueType the type of value to index.
\*/
template<typename KeyType, typename ValueType>
class ResourceManager
{
public:
	using PathType = std::string;
	/**Construct a new value in its final resting place in the singleton.*/
	template<typename...Args>
	ValueType& Emplace(const KeyType& key, const std::string& path,
		Args&&...args);
	/**Pop an object that matches the key off the map.*/
	void Pop(const KeyType& key);
	/**Check if somthing exists with this Key.*/
	bool Exists(const KeyType& key);
	/**Check if something exists with this path.
	\return An std::pair with first= exists or not, second = the key that
	already owns that resource.*/
	std::pair<bool, KeyType> FindExistingResource(const std::string& path);
	/**Get a vale from this manager.*/
	ValueType& Get(const KeyType& key);
	/**Shortcut for Get*/
	ValueType& operator[](const KeyType& key);
protected:
	/**the key, location pair.*/
	using PathValuePair = std::pair <PathType, std::shared_ptr<ValueType>>;
	/**Hide the default constructor*/
	ResourceManager();
	/**Deleted this constructor*/
	ResourceManager(const ResourceManager& other) = delete;
	/**Deleted this constructor*/
	ResourceManager(ResourceManager&& other) = delete;
	/**Deleted this operator=*/
	void operator=(const ResourceManager& other) = delete;
	/**Deleted this operator=*/
	void operator=(ResourceManager&& other) = delete;
	/**There is not much to destroy here.*/
	virtual ~ResourceManager();
	/**Get the name of this manager.*/
	virtual inline std::string GetName();

	/**The data in the form of a triplet of Key,Path,Value*/
	std::map<KeyType, PathValuePair> _data;

};

template<typename KeyType, typename ValueType>
inline void ResourceManager<KeyType, ValueType>::Pop(const KeyType & key)
{
	if (Exists(key))
	{
		cg::logger::log_note(2, GetName(),
			"(Pop):Popping the value indexed with ", key, ".");
		_data.erase(key);
	}
	else
	{
		cg::logger::log_error(GetName(), "(Pop):Resource does not exist.",
			key);
		throw std::runtime_error("Could not erase resource.");
	}
}

template<typename KeyType, typename ValueType>
inline bool ResourceManager<KeyType, ValueType>::Exists(const KeyType & key)
{
	bool exists = (_data.count(key) > 0);
	if (exists)
	{
		cg::logger::log_note(1, GetName(), "(Exists):Resource does exist. ",
			key);
	}
	else {
		cg::logger::log_note(1, GetName(), "(Exists):Resource does not exist. "
			, key);
	}
	return exists;
}

template<typename KeyType, typename ValueType>
inline std::pair<bool, KeyType> ResourceManager<KeyType, ValueType>::
FindExistingResource(const std::string & path)
{
	for (const auto& e : _data)
	{
		if (e.second.first == path)
		{
			cg::logger::log_error(GetName(), "(FindExistingResource):Resource",
				" exists under different key (", e.first, ")");
			return std::make_pair(true, e.first);
		}
	}
	cg::logger::log_note(1, GetName(), "(FindExistingResource):Resource does ",
		"not exist. ", path);
	return std::make_pair(false, KeyType());
}

template<typename KeyType, typename ValueType>
inline ValueType & ResourceManager<KeyType, ValueType>::Get(
	const KeyType & key)
{
	if (Exists(key))
	{
		cg::logger::log_note(1, GetName(), "(Get):Returning resource. "
			, key);
		return *_data[key].second;
	}
	else
	{
		cg::logger::log_note(1, GetName(), "(Get):Resource does not exist. "
			, key);
		throw std::runtime_error("Resource does not exist.");
	}
}

template<typename KeyType, typename ValueType>
inline ValueType & ResourceManager<KeyType, ValueType>::operator[](
	const KeyType & key)
{
	return Get(key);
}

template<typename KeyType, typename ValueType>
inline ResourceManager<KeyType, ValueType>::ResourceManager()
{
	cg::logger::log_note(3, "A manager was created.");
}

template<typename KeyType, typename ValueType>
inline ResourceManager<KeyType, ValueType>::~ResourceManager()
{
	cg::logger::log_note(3, "A manager was destroyed.");
}

template<typename KeyType, typename ValueType>
inline std::string ResourceManager<KeyType, ValueType>::GetName()
{
	return "ResourceManager (BASE)";
}
template<typename KeyType, typename ValueType>
template<typename ...Args>
inline ValueType & ResourceManager<KeyType, ValueType>::Emplace(
	const KeyType & key, const std::string & path, Args && ...args)
{
	if (Exists(key))
	{
		cg::logger::log_error(GetName(), "(Emplace): A resource with that key",
			" exists already.", key);
		throw std::runtime_error("A key with that name already exists.");
	}
	auto alternate = FindExistingResource(path);
	if (alternate.first)
	{
		cg::logger::log_error(GetName(), "(Emplace):Resource already exists.",
			key, " @ ", path);
		_data[key].second = _data[alternate.second].second;
		return *_data[key].second;
	}
	else
	{
		cg::logger::log_note(3, GetName(), "(Emplace):Adding resource. ", key,
			" @ ", path);
		_data[key].first = path;
		_data[key].second =
			std::make_shared<ValueType>(std::forward<Args>(args)...);
		return *_data[key].second;
	}
}




#endif //RESOURCEMANAGER_HPP

