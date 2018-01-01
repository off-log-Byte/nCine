#ifndef CLASS_NCINE_HASHMAP
#define CLASS_NCINE_HASHMAP

#include "Array.h"
#include "List.h"
#include "HashFunctions.h"

namespace ncine {

template <class K, class T, class HashFunc, bool IsConst> class HashMapIterator;
template <class K, class T, class HashFunc, bool IsConst> struct HelperTraits;
class String;

/// The template class for the node stored inside a hashmap
template <class K, class T>
class HashMapNode
{
  public:
	hash_t hash;
	K key;
	T value;

	HashMapNode() : hash(0) { }
	HashMapNode(hash_t hh, const K &kk)
		: hash(hh), key(kk) { }
};

/// A template based hashmap implementation with separate chaining and list head cell
template <class K, class T, class HashFunc>
class HashMap
{
  public:
	/// Iterator type
	typedef HashMapIterator<K, T, HashFunc, false> Iterator;
	/// Constant iterator type
	typedef HashMapIterator<K, T, HashFunc, true> ConstIterator;

	explicit HashMap(unsigned int size);
	~HashMap() { clear(); }

	/// Copy constructor
	HashMap(const HashMap &other);
	/// Copy-and-swap assignment operator
	HashMap &operator=(HashMap other);

	/// Swaps two hashmaps without copying their data
	inline void swap(HashMap &first, HashMap &second)
	{
		nc::swap(first.buckets_, second.buckets_);
		nc::swap(first.hashFunc_, second.hashFunc_);
	}

	/// Returns an iterator to the first element
	Iterator begin();
	/// Returns an iterator to past the last element
	Iterator end();

	/// Returns a constant iterator to the first element
	ConstIterator begin() const;
	/// Returns a constant iterator to past the last lement
	ConstIterator end() const;

	/// Subscript operator
	inline T &operator[](const K &key) { return retrieveBucket(key).findOrAdd(hashFunc_(key), key); }

	/// Returns the number of buckets
	inline unsigned int bucketAmount() const { return buckets_.size(); }
	/// Returns the size of the bucket for the hash generated by the specified key
	inline unsigned int bucketSize(const K &key) const { return retrieveBucket(key).size(); }
	/// Returns the hash of a given key
	inline hash_t hash(const K &key) const { return hashFunc_(key); }

	/// Clears the hashmap
	void clear();
	/// Checks whether an element is in the hashmap or not
	inline bool contains(const K &key, T &returnedValue) const { return retrieveBucket(key).contains(hashFunc_(key), key, returnedValue); }
	/// Checks whether an element is in the hashmap or not
	/*! \note Prefer this method if copying `T` is expensive, but always check the validity of returned pointer. */
	inline T *find(const K &key) { return retrieveBucket(key).find(hashFunc_(key), key); }
	/// Removes a key from the hashmap, if it exists
	inline bool remove(const K &key) { return retrieveBucket(key).remove(hashFunc_(key), key); }

  private:
	/// The bucket class for the hashmap, with separate chaining and list head cell
	class HashBucket
	{
	  public:
		HashBucket() : size_(0) { }
		unsigned int size() const { return size_; }
		void clear();
		bool contains(hash_t hash, const K &key, T &returnedValue) const;
		T *find(hash_t hash, const K &key);
		T &findOrAdd(hash_t hash, const K &key);
		bool remove(hash_t hash, const K &key);

	  private:
		unsigned int size_;
		/// Separate chaining with head cell
		HashMapNode<K, T> firstNode_;
		/// Separate chaining with a linked list
		List<HashMapNode<K, T> > collisionList_;

		friend class HashMapIterator<K, T, HashFunc, false>;
		friend class HashMapIterator<K, T, HashFunc, true>;
		friend class HashMap<K, T, HashFunc>;
	};

	const HashBucket &retrieveBucket(const K &key) const;
	HashBucket &retrieveBucket(const K &key);

	Array<HashBucket> buckets_;
	HashFunc hashFunc_;

	friend class HashMapIterator<K, T, HashFunc, false>;
	friend class HashMapIterator<K, T, HashFunc, true>;
	friend struct HelperTraits<K, T, HashFunc, false>;
	friend struct HelperTraits<K, T, HashFunc, true>;
};

template <class K, class T, class HashFunc>
inline typename HashMap<K, T, HashFunc>::Iterator HashMap<K, T, HashFunc>::begin()
{
	Iterator iterator(this, 0, buckets_[0].collisionList_.begin(), true);
	if (buckets_[0].size_ == 0)
		++iterator;

	return iterator;
}

template <class K, class T, class HashFunc>
inline typename HashMap<K, T, HashFunc>::Iterator HashMap<K, T, HashFunc>::end()
{
	const unsigned int lastIndex = buckets_.size() - 1;
	return Iterator(this, lastIndex, buckets_[lastIndex].collisionList_.end(), false);
}

template <class K, class T, class HashFunc>
inline typename HashMap<K, T, HashFunc>::ConstIterator HashMap<K, T, HashFunc>::begin() const
{
	ConstIterator iterator(this, 0, buckets_[0].collisionList_.begin(), true);
	if (buckets_[0].size_ == 0)
		++iterator;

	return iterator;
}

template <class K, class T, class HashFunc>
inline typename HashMap<K, T, HashFunc>::ConstIterator HashMap<K, T, HashFunc>::end() const
{
	const unsigned int lastIndex = buckets_.size() - 1;
	return ConstIterator(this, lastIndex, buckets_[lastIndex].collisionList_.end(), false);
}

template <class K, class T, class HashFunc>
void HashMap<K, T, HashFunc>::HashBucket::clear()
{
	collisionList_.clear();
	size_ = 0;
}

template <class K, class T, class HashFunc>
bool HashMap<K, T, HashFunc>::HashBucket::contains(hash_t hash, const K &key, T &returnedValue) const
{
	if (size_ == 0)
		return false;

	bool found = false;
	if (firstNode_.hash == hash && firstNode_.key == key)
	{
		// The item has been found in the direct access node
		found = true;
		returnedValue = firstNode_.value;
	}
	else
	{
		for (typename List<HashMapNode<K, T> >::ConstIterator i = collisionList_.begin(); i != collisionList_.end(); ++i)
		{
			if ((*i).hash == hash && (*i).key == key)
			{
				// The item has been found in the list
				found = true;
				returnedValue = (*i).value;
				break;
			}
		}
	}

	return found;
}

template <class K, class T, class HashFunc>
T *HashMap<K, T, HashFunc>::HashBucket::find(hash_t hash, const K &key)
{
	if (size_ == 0)
		return NULL;

	if (firstNode_.hash == hash && firstNode_.key == key)
	{
		// The item has been found in the direct access node
		return &firstNode_.value;
	}
	else
	{
		for (typename List<HashMapNode<K, T> >::Iterator i = collisionList_.begin(); i != collisionList_.end(); ++i)
		{
			if ((*i).hash == hash && (*i).key == key)
			{
				// The item has been found in the list
				return &(*i).value;
			}
		}
	}

	return NULL;
}

template <class K, class T, class HashFunc>
T &HashMap<K, T, HashFunc>::HashBucket::findOrAdd(hash_t hash, const K &key)
{
	if (size_ == 0)
	{
		// Early-out if the bucket is empty
		firstNode_.hash = hash;
		firstNode_.key = key;
		size_++;
		return firstNode_.value;
	}

	if (firstNode_.hash == hash && firstNode_.key == key)
	{
		// The item has been found in the direct access node
		return firstNode_.value;
	}
	else
	{
		for (typename List<HashMapNode<K, T> >::Iterator i = collisionList_.begin(); i != collisionList_.end(); ++i)
		{
			if ((*i).hash == hash && (*i).key == key)
			{
				// The item has been found in the list
				return (*i).value;
			}
		}
	}

	// The item has not been found, a new entry is created at the end of the list
	size_++;
	collisionList_.pushBack(HashMapNode<K, T>(hash, key));
	return (*collisionList_.rBegin()).value;
}

/*! \return True if the element has been found and removed */
template <class K, class T, class HashFunc>
bool HashMap<K, T, HashFunc>::HashBucket::remove(hash_t hash, const K &key)
{
	// Early-out if the bucket is empty
	if (size_ == 0)
		return false;

	bool found = false;
	if (firstNode_.hash == hash && firstNode_.key == key)
	{
		// The item has been found in the direct access node
		found = true;

		// Bring the first element of the list, if any, as direct access node
		if (collisionList_.isEmpty() == false)
		{
			firstNode_ = collisionList_.front();
			collisionList_.popFront();
		}
		size_--;
	}
	else
	{
		for (typename List<HashMapNode<K, T> >::ConstIterator i = collisionList_.begin(); i != collisionList_.end(); ++i)
		{
			if ((*i).hash == hash && (*i).key == key)
			{
				// The item has been found in the list
				found = true;
				i = collisionList_.erase(i);
				size_--;

				// The item has been removed and the iterator is no longer valid
				break;
			}
		}
	}

	return found;
}

template <class K, class T, class HashFunc>
HashMap<K, T, HashFunc>::HashMap(unsigned int size)
	: buckets_(size, ArrayMode::FIXED_CAPACITY)
{
	for (unsigned int i = 0; i < size; i++)
		buckets_[i] = HashBucket();
}

template <class K, class T, class HashFunc>
HashMap<K, T, HashFunc>::HashMap(const HashMap<K, T, HashFunc> &other)
	: buckets_(other.buckets_), hashFunc_(other.hashFunc_)
{

}

/*! The parameter should be passed by value for the idiom to work. */
template <class K, class T, class HashFunc>
HashMap<K, T, HashFunc> &HashMap<K, T, HashFunc>::operator=(HashMap<K, T, HashFunc> other)
{
	swap(*this, other);
	return *this;
}

template <class K, class T, class HashFunc>
void HashMap<K, T, HashFunc>::clear()
{
	for (unsigned int i = 0; i < buckets_.size(); i++)
		buckets_[i].clear();
}

template <class K, class T, class HashFunc>
inline const typename HashMap<K, T, HashFunc>::HashBucket &HashMap<K, T, HashFunc>::retrieveBucket(const K &key) const
{
	const hash_t hash = hashFunc_(key);
	const unsigned int index = hash % buckets_.size();
	return buckets_[index];
}

template <class K, class T, class HashFunc>
inline typename HashMap<K, T, HashFunc>::HashBucket &HashMap<K, T, HashFunc>::retrieveBucket(const K &key)
{
	const hash_t hash = hashFunc_(key);
	const unsigned int index = hash % buckets_.size();
	return buckets_[index];
}

/// A way to simulate C++11 alias templates
template <class T>
struct StringHashMap
{
	typedef HashMap<String, T, SaxHashFunc<String> > Type;
	typedef typename Type::Iterator Iterator;
	typedef typename Type::ConstIterator ConstIterator;
};

}

#endif
