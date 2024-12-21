#include <iostream>
#include <string>

using namespace std;

struct Node {
    string title; // Tên của chương, mục, tiểu mục
    int pageCount;   // Số trang của mục này
    Node* parent;    // Node cha
    Node* children[100]; // Danh sách các mục con
    int childCount;  // Số lượng mục con

    Node(const string& t, int pc, Node* p = nullptr)
        : title(t), pageCount(pc), parent(p), childCount(0) {
        for (int j = 0; j < 100; ++j) children[j] = nullptr;
    }
};

class Book {
private:
    Node* root;

    void countChapters(Node* node, int& chapterCount) {
        if (!node) return;
        for (int i = 0; i < node->childCount; ++i) {
            if (node->children[i]->parent == root) // Là chương nếu cha là gốc
                chapterCount++;
            countChapters(node->children[i], chapterCount);
        }
    }

    void findLongestChapter(Node* node, int& maxPages, string& longestChapter) {
        if (!node) return;
        for (int i = 0; i < node->childCount; ++i) {
            if (node->children[i]->parent == root && node->children[i]->pageCount > maxPages) {
                maxPages = node->children[i]->pageCount;
                longestChapter = node->children[i]->title;
            }
            findLongestChapter(node->children[i], maxPages, longestChapter);
        }
    }

    bool removeNode(Node* node, const string& titleToRemove) {
        for (int i = 0; i < node->childCount; ++i) {
            if (node->children[i]->title == titleToRemove) {
                delete node->children[i];
                for (int j = i; j < node->childCount - 1; ++j) {
                    node->children[j] = node->children[j + 1];
                }
                node->childCount--;
                return true;
            } else if (removeNode(node->children[i], titleToRemove)) {
                return true;
            }
        }
        return false;
    }

    void updatePageCounts(Node* node) {
        if (!node) return;
        node->pageCount = 0;
        for (int i = 0; i < node->childCount; ++i) {
            updatePageCounts(node->children[i]);
            node->pageCount += node->children[i]->pageCount;
        }
    }

    void printTree(Node* node, int level = 0) {
        if (!node) return;
        for (int i = 0; i < level; ++i) cout << "  ";
        cout << "- " << node->title << " (" << node->pageCount << " pages)\n";
        for (int i = 0; i < node->childCount; ++i) {
            printTree(node->children[i], level + 1);
        }
    }

public:
    Book() : root(new Node("Book", 0)) {}

    ~Book() { delete root; }

    void addChapter(const string& title, int pageCount) {
        root->children[root->childCount++] = new Node(title, pageCount, root);
    }

    void addSection(const string& chapterTitle, const string& title, int pageCount) {
        for (int i = 0; i < root->childCount; ++i) {
            if (root->children[i]->title == chapterTitle) {
                Node* chapter = root->children[i];
                chapter->children[chapter->childCount++] = new Node(title, pageCount, chapter);
                chapter->pageCount += pageCount;
                break;
            }
        }
    }

    int getChapterCount() {
        int chapterCount = 0;
        countChapters(root, chapterCount);
        return chapterCount;
    }

    string getLongestChapter() {
        int maxPages = 0;
        string longestChapter;
        findLongestChapter(root, maxPages, longestChapter);
        return longestChapter;
    }

    void removeItem(const string& title) {
        if (removeNode(root, title)) {
            updatePageCounts(root);
        }
    }

    void printBook() {
        printTree(root);
    }
};

int main() {
    Book myBook;
    myBook.addChapter("Chapter 1", 30);
    myBook.addChapter("Chapter 2", 40);
    myBook.addSection("Chapter 1", "Section 1.1", 15);
    myBook.addSection("Chapter 1", "Section 1.2", 10);
    myBook.addSection("Chapter 2", "Section 2.1", 20);

    cout << "Initial Book Structure:\n";
    myBook.printBook();

    cout << "\nTotal Chapters: " << myBook.getChapterCount() << "\n";
    cout << "Longest Chapter: " << myBook.getLongestChapter() << "\n";

    myBook.removeItem("Section 1.1");

    cout << "\nBook Structure After Removal:\n";
    myBook.printBook();

    return 0;
}
