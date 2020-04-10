# [基本手写代码](https://blog.csdn.net/qq_41572503/article/details/88542859)

> * 二分查找
> * 插入排序
> * 堆排序
> * 归并排序
> * 快速排序（随机单排，双排）
> * 单例模式
> * 非递归二叉树遍历
> * [各类排序算法](https://www.cnblogs.com/onepixel/articles/7674659.html)

```c++
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;
/*
冒泡排序
1、比较相邻的元素。如果第一个比第二个大，就交换它们两个；
2、对每一对相邻元素作同样的工作，从开始第一对到结尾的最后一对，这样在最后的元素应该会是最大的数；
3、针对所有的元素重复以上的步骤
最好情况：如果元素本来就是有序的，那么一趟冒泡排序既可以完成排序工作 O(n)
最差情况：数据元素本来就是逆序 O(n2)
平均时间复杂度为O(n2)
*/
vector<int> buble_sort(vector<int> &nums)
{
	for (int i = 0; i < nums.size(); i++)
	{
		for (int j = 0; j < nums.size() - 1 - i; j++)
		{
			if (nums[j] > nums[j+1]) //比较相邻的元素。如果第一个比第二个大，就交换它们两个
			{
				swap(nums[j], nums[j+1]);
			}
		}
	}
	return nums;
}

/*
插入排序
1、从第一个元素开始，该元素可以认为已经被排序；
2、取出下一个元素，在已经排序的元素序列中从后向前扫描；
3、如果该元素（已排序）大于新元素，将该元素移到下一位置；
4、重复步骤3，直到找到已排序的元素小于或者等于新元素的位置；
5、将新元素插入到该位置后；
6、重复步骤2~5。
最好情况：如果元素本来就是有序的，那么一趟冒泡排序既可以完成排序工作 O(n)
最差情况：数据元素本来就是逆序 O(n2)
平均时间复杂度为O(n2)
*/

vector<int> insert_sort(vector<int> &nums)
{
	for (int i = 0; i < nums.size() - 1; i++)
	{
		int j = i + 1;
		while (j > 0 && nums[j] < nums[j - 1])
		{
			swap(nums[j], nums[j - 1]);
			j--;
		}
	}
	return nums;
}



int main()
{
	vector<int> nums= { 5,4,2,3,1,6 };

	vector<int> result = insert_sort(nums);

	for (int i = 0; i < result.size(); i++)
	{
		cout << result[i] << ' ';
	}
	cout << endl;

	return 0;
}
```

