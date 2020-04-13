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
void buble_sort(vector<int> &nums)
{
	for (int i = 0; i < nums.size(); i++)
	{
		for (int j = 0; j < nums.size() - 1 - i; j++)
		{
			if (nums[j] > nums[j + 1]) //比较相邻的元素。如果第一个比第二个大，就交换它们两个
			{
				swap(nums[j], nums[j + 1]);
			}
		}
	}
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

void insert_sort(vector<int> &nums)
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
}


/*
快速排序使用分治法来把一个串（list）分为两个子串（sub-lists）。具体算法描述如下：

1、从数列中挑出一个元素，称为 “基准”（pivot）；
2、重新排序数列，所有元素比基准值小的摆放在基准前面，所有元素比基准值大的摆在基准的后面（相同的数可以到任一边）。
3、在这个分区退出之后，该基准就处于数列的中间位置。这个称为分区（partition）操作；
4、递归地（recursive）把小于基准值元素的子数列和大于基准值元素的子数列排序。
最好情况：Partition函数每次恰好能均分序列O(n*logn)
最差情况：每次划分只能将序列分为一个元素与其他元素两部分O(n2)
平均情况：O(n*logn)
*/
void quick_sort(vector<int> &nums, int left, int right)
{
	if (left >= right) return;
	int i = left; int j = right + 1;
	int pivot = nums[left];
	while (1)
	{
		while (i < right && nums[++i] < pivot) {} //从右往左，找到下一个大于pivot的数

		while (j > left&& nums[--j] > pivot) {} //从左往右，找到下一个小于pivot的数

		if (i >= j) break; //如果大数在小数的右边，则不用交换
		swap(nums[i], nums[j]);
	}
	swap(nums[left], nums[j]);
	if (left < right)
	{
		quick_sort(nums, left, j - 1);
		quick_sort(nums, j + 1, right);
	}
}

int main()
{
	vector<int> nums = { 0,4,3,4,9 };

	quick_sort(nums, 0, nums.size() - 1);

	for (int i = 0; i < nums.size(); i++)
	{
		cout << nums[i] << ' ';
	}
	cout << endl;

	return 0;
}
```

