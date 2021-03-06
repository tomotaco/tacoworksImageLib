#ifndef __COMBINATION_H_INCLUDED
#define __COMBINATION_H_INCLUDED

#include <list>
#include <vector>

/**
	rListValues で渡されたリストから、numElementsCombination 個の要素からなる
	全ての組み合わせについてoperation() を呼び出す。
*/
namespace Combination
{
	template<typename TypeValue, typename TypeOperation>
	bool applyOperationForAllCombination(const std::list<TypeValue>& rListValues, unsigned int numElementsCombination, TypeOperation& rOperation)
	{
		unsigned int numPointsInList = rListValues.size();
		if (numPointsInList < numElementsCombination) return false;

		// ランダムアクセス可能な vector にコピー
		std::vector<TypeValue> points(rListValues.begin(), rListValues.end());

		std::list<std::vector<TypeValue>::const_iterator> stackIndices;

		// 先頭の要素をスタックに積んでおく
		stackIndices.push_back(points.begin());
		while (stackIndices.size() < numElementsCombination) {
			stackIndices.push_back(stackIndices.back() + 1);
		}

		while (stackIndices.size() > 0) {
			std::vector<TypeValue>::const_iterator& rIterator = stackIndices.back();
			if (rIterator == points.end()) {
				stackIndices.pop_back();
				// スタック上のイテレータが点列の最後を指していたら外していく
				unsigned int sizeStack = stackIndices.size();
				while (sizeStack > 0 && stackIndices.back() == points.begin() + (numPointsInList - numElementsCombination + sizeStack - 1)) {
					stackIndices.pop_back();
					sizeStack = stackIndices.size();
				}
				// 空になったら終了
				if (stackIndices.size() > 0) {
					// 残ったスタックの頭に入っているイテレータを一つ進める
					stackIndices.back() ++;
					while (stackIndices.size() < numElementsCombination) {
						stackIndices.push_back(stackIndices.back() + 1);
					}
				}
			} else {
				std::list<TypeValue> listValues;
				std::list<std::vector<TypeValue>::const_iterator>::const_iterator it = stackIndices.begin();
				for (; it != stackIndices.end(); ++ it) {
					listValues.push_back(*(*it));
				}
				rOperation(listValues);

				rIterator ++;
			}
		}

		return true;
	}
};

#endif
