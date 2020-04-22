# [基本手写代码](https://blog.csdn.net/qq_41572503/article/details/88542859)

> * 二分查找
> * 插入排序
> * 堆排序
> * 归并排序
> * 快速排序（随机单排，双排）
> * 单例模式# [基本手写代码](https://blog.csdn.net/qq_41572503/article/details/88542859)

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

	quick_sort(nums, left, j - 1);
	quick_sort(nums, j + 1, right);
}

/*
归并排序是建立在归并操作上的一种有效的排序算法。该算法是采用分治法（Divide and Conquer）的一个非常典型的应用。
将已有序的子序列合并，得到完全有序的序列；即先使每个子序列有序，再使子序列段间有序。
若将两个有序表合并成一个有序表，称为2-路归并。
1、把长度为n的输入序列分成两个长度为n/2的子序列；
2、对这两个子序列分别采用归并排序；
3、将两个排序好的子序列合并成一个最终的排序序列。
*/
void merge(vector<int> &nums, int left,int right, int rightend) //将两个排序好的子序列合并成一个最终的排序序列
{
	vector<int> temp_nums(rightend - left + 1);
	int leftend = right - 1;
	int temp_left = left;

	int count = 0;
	while (left <= leftend && right <= rightend) //比较两个子序列中元素的大小并按顺序存在临时数组中
	{
		if (nums[left] <= nums[right])
		{
			temp_nums[count++] = nums[left++];
		}
		else
		{
			temp_nums[count++] = nums[right++];
		}
	}

	while (left <= leftend)
	{
		temp_nums[count++] = nums[left++];
	}

	while (right <= rightend)
	{
		temp_nums[count++] = nums[right++];
	}

	for (int i = temp_left; i <= rightend; i++)
	{
		nums[i] = temp_nums[i - temp_left];//从临时数组中再拷贝回原来的数组
	}
}

void merge_sort(vector<int> &nums, int left, int rightend)
{
	int mid = (rightend - left) / 2 + left;
	int leftend = mid; int right = mid + 1;
	if (left < rightend)
	{
		merge_sort(nums, left, leftend); //递归归并，直到 left < rightend
		merge_sort(nums, right, rightend);
		merge(nums, left, right, rightend); //从长度为1的序列再合并成大序列
	}
}


/*
希尔排序先将整个待排序的记录序列分割成为若干子序列分别进行直接插入排序，具体算法描述：
选择一个增量序列t1，t2，…，tk，其中ti>tj，tk=1；
按增量序列个数k，对序列进行k 趟排序；
每趟排序，根据对应的增量ti，将待排序列分割成若干长度为m 的子序列，分别对各子表进行直接插入排序。
仅增量因子为1 时，整个序列作为一个表来处理，表长度即为整个序列的长度。
*/

void shell_Sort(vector<int> &nums)  //nums -- 待排序的数组
{
	int n = nums.size();
	for (int gap = n / 2; gap > 0; gap /= 2) //进行分组，最开始的增量为开始的一半，每次减为原来的一半。
	{
		for (int i = 0; i < gap; i++) // 共gap个组，对每一组都执行直接插入排序
		{
			for (int j = i; j + gap < n; j += gap)
			{
				int k = j + gap;
				while (k >gap&&nums[k]<nums[k -gap])
				{
					swap(nums[k], nums[k - gap]);
					k -= gap;
				}
			}
		}
	}
}


int main()
{
	vector<int> nums = { 0,4,3,4,9 };

	merge_sort(nums, 0, nums.size() - 1);

	for (int i = 0; i < nums.size(); i++)
	{
		cout << nums[i] << ' ';
	}
	cout << endl;

	return 0;
}
```



## [堆排序](https://blog.csdn.net/u010452388/article/details/81283998?depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2&utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2)

1. 将无需序列构建成一个堆，根据升序降序需求选择大顶堆或小顶堆;
2. 将堆顶元素与末尾元素交换，将最大元素"沉"到数组末端；
3. 重新调整结构，使其满足堆定义，然后继续交换堆顶元素与当前末尾元素，反复执行调整+交换步骤，直到整个序列有序。

- 堆是具有以下性质的完全二叉树：每个结点的值都大于或等于其左右孩子结点的值，称为大顶堆；

- 或者每个结点的值都小于或等于其左右孩子结点的值，称为小顶堆。
- 大顶堆：arr[i] >= arr[2i+1] && arr[i] >= arr[2i+2]
- 小顶堆：arr[i] <= arr[2i+1] && arr[i] <= arr[2i+2]

## [计数排序](https://www.jianshu.com/p/86c2375246d7)

1. 根据待排序集合中最大元素和最小元素的差值范围，申请额外空间；
2. 遍历待排序集合，将每一个元素出现的次数记录到元素值对应的额外空间内；
3. 对额外空间内数据进行计算，得出每一个元素的正确位置；
4. 将待排序集合每一个元素移动到计算得出的正确位置上。

- **当数列最大最小值差距过大时，并不适用于计数排序**

- **当数列元素不是整数时，并不适用于计数排序**

## [桶排序](https://www.jianshu.com/p/204ed43aec0c)

1. 根据待排序集合中最大元素和最小元素的差值范围和映射规则，确定申请的桶个数；
2. 遍历待排序集合，将每一个元素移动到对应的桶中；
3. 对每一个桶中元素进行排序，并移动到已排序集合中。

## [基数排序](https://www.cnblogs.com/skywang12345/p/3603669.html#a42)

1. 将所有待比较数值统一为同样的数位长度，数位较短的数前面补零。
2. 然后，从最低位开始，依次进行一次排序。
3. 这样从最低位排序一直到最高位排序完成以后, 数列就变成一个有序序列。
