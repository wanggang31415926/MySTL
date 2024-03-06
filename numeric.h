#ifndef MY_STL_NUMERIC_H_
#define MY_STL_NUMERIC_H_

// ���ͷ�ļ������� mystl ����ֵ�㷨

#include "iterator.h"
namespace mystl
{

/*****************************************************************************************/
// accumulate
// �汾1���Գ�ֵ init ��ÿ��Ԫ�ؽ����ۼ�
// �汾2���Գ�ֵ init ��ÿ��Ԫ�ؽ��ж�Ԫ����
/*****************************************************************************************/
// �汾1
template <typename InputIter, typename T>
T accumulate(InputIter first, InputIter last, T init)
{
    for (; first != last; ++first)
    {
        init += *first;
    }
    return init;
}

// �汾2
template <typename InputIter, typename T, typename BinaryOp>
T accumulate(InputIter first, InputIter last, T init, BinaryOp binary_op)
{
    for (; first != last; ++first)
    {
        init = binary_op(init, *first);
    }
    return init;
}

/*****************************************************************************************/
// adjacent_difference
// �汾1����������Ԫ�صĲ�ֵ��������浽�� result Ϊ��ʼ��������
// �汾2���Զ�������Ԫ�صĶ�Ԫ����
/*****************************************************************************************/
// �汾1
template <typename InputIter, typename OutputIter>
OutputIter adjacent_difference(InputIter first, InputIter last, OutputIter result)
{
    if (first == last)  return result;
    *result = *first;  // ��¼��һ��Ԫ��
    auto value = *first;
    while (++first != last)
    {
        auto tmp = *first;
        *++result = tmp - value;
        value = tmp;
    }
    return ++result;
}

// �汾2
template <typename InputIter, typename OutputIter, typename BinaryOp>
OutputIter adjacent_difference(InputIter first, InputIter last, OutputIter result,
    BinaryOp binary_op)
{
    if (first == last)  return result;
    *result = *first;  // ��¼��һ��Ԫ��
    auto value = *first;
    while (++first != last)
    {
        auto tmp = *first;
        *++result = binary_op(tmp, value);
        value = tmp;
    }
    return ++result;
}

/*****************************************************************************************/
// inner_product
// �汾1���� init Ϊ��ֵ����������������ڻ�   
// �汾2���Զ��� operator+ �� operator*
/*****************************************************************************************/
// �汾1
template <typename InputIter1, typename InputIter2, typename T>
T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2, T init)
{
    for (; first1 != last1; ++first1, ++first2)
    {
        init = init + (*first1 * *first2);
    }
    return init;
}

// �汾2
template <typename InputIter1, typename InputIter2, typename T, typename BinaryOp1, typename BinaryOp2>
T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2, T init,
    BinaryOp1 binary_op1, BinaryOp2 binary_op2)
{
    for (; first1 != last1; ++first1, ++first2)
    {
        init = binary_op1(init, binary_op2(*first1, *first2));
    }
    return init;
}

/*****************************************************************************************/
// iota
// ���[first, last)���� value Ϊ��ֵ��ʼ����
/*****************************************************************************************/
template <typename ForwardIter, typename T>
void iota(ForwardIter first, ForwardIter last, T value)
{
    while (first != last)
    {
        *first++ = value;
        ++value;
    }
}


/*****************************************************************************************/
// partial_sum
// �汾1������ֲ��ۼ���ͣ�������浽�� result Ϊ��ʼ��������
// �汾2�����оֲ������Զ����Ԫ����
/*****************************************************************************************/
template <class InputIter, class OutputIter>
OutputIter partial_sum(InputIter first, InputIter last, OutputIter result)
{
    if (first == last)  return result;
    *result = *first;  // ��¼��һ��Ԫ��
    auto value = *first;
    while (++first != last)
    {
        value = value + *first;
        *++result = value;
    }
    return ++result;
}

// �汾2
template <class InputIter, class OutputIter, class BinaryOp>
OutputIter partial_sum(InputIter first, InputIter last, OutputIter result,
    BinaryOp binary_op)
{
    if (first == last)  return result;
    *result = *first;  //��¼��һ��Ԫ��
    auto value = *first;
    while (++first != last)
    {
        value = binary_op(value, *first);
        *++result = value;
    }
    return ++result;
}


} // namespace mystl
#endif // !MY_STL_NUMERIC_H_

