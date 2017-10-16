#ifndef CLASSCHECKER_HPP_
#define CLASSCHECKER_HPP_

#include <set>

class PlayEntity;

class ClassChecker {

public:
	ClassChecker();
	~ClassChecker();

	template <class Type1, class Type2>
	bool isKindOfClass(Type2 entity) {
		if(!entity) {
			return false;
		}

		Type1 converted = dynamic_cast<Type1>(entity);
		if(converted != nullptr){
			return true;
		}

		return false;
	}
};

#endif /* CLASSCHECKER_HPP_ */
