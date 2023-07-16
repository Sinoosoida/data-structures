#ifndef MEMORY_MANAGER_HEAD_H_2023_02_10
#define MEMORY_MANAGER_HEAD_H_2023_02_10

#include <cstring>
#include <iostream>
#include <new>


int symbols_in_int(int input) {
    int output = 0;
    for (; input>0 ; input %= 10)
        output++;
}

template<class TYPE>
inline void ConstructElements(TYPE *pElements) {
    memset(reinterpret_cast<void *>(pElements), 0, sizeof(TYPE));
    ::new (reinterpret_cast<void *>(pElements)) TYPE;
}

template<class TYPE>
inline void DestructElements(TYPE *pElements) {
    pElements->~TYPE();
    memset(reinterpret_cast<void *>(pElements), 0, sizeof(TYPE));
}


namespace lab618 {
    template<class T>
    class CMemoryManager {
        private:
        struct block {
            // ������ ������ �����
            T *pdata;
            // ����� ���������� �����
            block *pnext;
            // ������ ��������� ������
            int firstFreeIndex;
            // ����� ����������� �����
            int usedCount;

//            void print(int block_size) {
//                const int symbols_per_line = 100;
//                const int multiplicity = 10;
//                int symbols_per_element = symbols_in_int(block_size);
//                std::string separator = "|";
//                std::string space = " ";
//                #define free_color "\33[38;5;0;48;5;255m"
//                #define allocated_color "\33[38;5;255;48;5;0m"
//                #define reset "\33[m"
//
//                int elements_per_line = (symbols_per_line - separator.length())/(separator.length()+symbols_per_element)/multiplicity*multiplicity;
//
//                std::cout << "����:\t" << this << "��������� �� ����:\t" << pnext << std::endl;
//                std::cout << "���������:\t" << usedCount << " ����� ��\t" << block_size << std::endl;
//                for (int i=0; i<(block_size+elements_per_line-1)/elements_per_line; i++) {
//                    std::cout << separator;
//                    for (int j=0 ; j<elements_per_line; j++) {
////                        printf(ANSI_COLOR1 "XXX" ANSI_RESET);
//                        for (int k=0 ; k<symbols_per_element- symbols_in_int(k); k++)
//                            printf(ANSI_COLOR1 "XXX" ANSI_RESET);
//                    }
//                }
//
//            }
        };

        public:
        class CException {
            //�� ������������
            public:
            CException() {
            }
        };

        public:
        /**
          _default_block_size - ���������� ��������� � ����� ������
          isDeleteElementsOnDestruct - ���������� �������� � ����������� ��������� ��� ��������� 
                                       �� ������� ��������������� �������� deleteObject ���������.
        */
        CMemoryManager(int _default_block_size, bool isDeleteElementsOnDestruct = false) : m_blkSize(_default_block_size),
                                                                                           m_pBlocks(nullptr),
                                                                                           m_pCurrentBlk(nullptr),
                                                                                           m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct) {}

        virtual ~CMemoryManager() {
            clear();
        }

        // �������� ����� ������ �������� �� ���������
        T *newObject() {//SGFM

            if (m_pBlocks == nullptr) {//���� ������ ��� - ������
                m_pBlocks = newBlock();
                m_pCurrentBlk = m_pBlocks;
            } else {//���� ����� ����, �� ���������� ��
                auto tmp = m_pBlocks;
                while ((tmp != nullptr) && (tmp->usedCount == m_blkSize)) {
                    tmp = tmp->pnext;
                }
                if (tmp == nullptr) {//���� ���������� ����� �� �������, �� ������
                    while (m_pCurrentBlk->pnext != nullptr) {
                        m_pCurrentBlk = m_pCurrentBlk->pnext;
                    }
                    m_pCurrentBlk->pnext =  newBlock();
                    m_pCurrentBlk = m_pCurrentBlk->pnext;
                }
            }

            //������ � ��� ���� ��������� ����

            int *FreeSpace = reinterpret_cast<int*> (m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex);
            int firstFreeIndex = *FreeSpace;//�����, ��� �������� � ��������� ������ - ������� ���������� ���������� ��������
            m_pCurrentBlk->firstFreeIndex = firstFreeIndex;
            m_pCurrentBlk->usedCount++;
            ConstructElements(FreeSpace);
            return reinterpret_cast<T*>(FreeSpace);
        }

        // ���������� ������� � ���������
        bool deleteObject(T *p) {
            //����� ����, ��� ����� �������
            int* pInt = reinterpret_cast<int*> (p);
            block *blk = m_pBlocks;
            while (blk && !(p >= blk->pdata && p < blk->pdata + m_blkSize)) {
                blk = blk->pnext;
            }
            if (blk == nullptr) {
                return false;
            }
            if (p >= blk->pdata && p < blk->pdata + m_blkSize) {//����� ����, ������ �������� ���������� ���������
                int FreeIndex = blk->firstFreeIndex;//���������� ������ �����
                while (FreeIndex != -1) {
                    if ((blk->pdata + FreeIndex) == p) {//���� ��������� ��������� �� ������ �����
                        return false;//�� ������� ��� ��� �����, �������� ������� �� ������
                    }
                    FreeIndex = *reinterpret_cast<int*>(blk->pdata + FreeIndex);
                }
                *pInt = blk->firstFreeIndex;
                blk->firstFreeIndex = p - blk->pdata;
                return true;
            } else {
                return false;
            }
        }

        // ������� ������, ������� �� m_isDeleteElementsOnDestruct
        void clear() {
            if (m_pBlocks == nullptr) {//���� ����� �� ���� ��������, �� � �������� ������
                return;
            }
            if (m_isDeleteElementsOnDestruct) {//���� �� ����� ������� �������� ��� ��������
                while (m_pBlocks != nullptr) {//������� ��� ��������
                    block *tmp = m_pBlocks->pnext;

                    deleteBlock(m_pBlocks);
                    m_pBlocks = tmp;
                }
            } else {// ���� ������� �������� ��� �������� �� �� �����
                bool* emptyMask = new bool[m_blkSize]; //������ �����, ��� ��������� ��� ���������� � ��������� ������� //TODO
                while (m_pBlocks != nullptr) {
                    block *tmp = m_pBlocks->pnext;
                    for(int i = 0; i < m_blkSize; i++) {
                        emptyMask[i] = false;
                    }
                    int FreeSpaceIndex = m_pBlocks->firstFreeIndex;
                    while(FreeSpaceIndex != -1) {
                        emptyMask[FreeSpaceIndex] = true;
                    }
                    deleteBlock(m_pBlocks, emptyMask);
                    m_pBlocks = tmp;
                }
                delete[] emptyMask;
            }
            m_pBlocks = nullptr;
            m_pCurrentBlk = nullptr;
        }

//        private:
        // ������� ����� ���� ������. ����������� � newObject
        block *newBlock() {
            block* new_block = new block;
            new_block->usedCount = 0;
            new_block->firstFreeIndex = 0;
            new_block->pnext = nullptr;
            new_block->pdata = new T[m_blkSize];
            for (int i=0 ; i<m_blkSize; i++) {
                *(reinterpret_cast<int*>(new_block->pdata+i)) = i+1;
            }
            *(reinterpret_cast<int*>(new_block->pdata+m_blkSize-1)) = -1;

            return new_block;
        }

        // ���������� ������ ����� ������. ����������� � clear
        void deleteBlock(block *p, bool* emptyMask = nullptr) {
//            std::cout << p.pdata << stD::endl;
            if (m_isDeleteElementsOnDestruct) {
                delete[] p->pdata;
                delete p;
            } else {
                for(int i = 0; i < m_blkSize; i++) {
                    if(!emptyMask[i]) {
                        DestructElements(p->pdata + i);
                    }
                }

                delete[] p->pdata;
                delete p;
            }
        }

        // ������ �����, ������� ��������� ���� T ����� ��������� � ����� �����
        int m_blkSize;
        // ������ ������������ ������ ������
        block *m_pBlocks;
        // ������� ����
        block *m_pCurrentBlk;
        // ������� �� �������� ��� ������������
        bool m_isDeleteElementsOnDestruct;
    };
};// namespace lab618

#endif// #define MEMORY_MANAGER_HEAD_H_2023_02_10
