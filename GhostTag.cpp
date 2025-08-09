#include <iostream>
#include <string>
using namespace std;

class Post {
protected:
    int postID;
    string content;
    int likes;
    string* comments;
    int commentCount;
    static int nextPostID;
public:
    Post(string c) : content(c), likes(0), commentCount(0) {
        postID = nextPostID++;
        comments = new string[0];
    }
    virtual ~Post() { delete[] comments; }
    virtual void display() const = 0;
    virtual void like(int count = 1) {
        likes += count; 
    }
    virtual Post* clone() const = 0;
    void addComment(string c) {
        string* temp = new string[commentCount + 1];
        for (int i = 0; i < commentCount; ++i) temp[i] = comments[i];
        temp[commentCount++] = c;
        delete[] comments;
        comments = temp;
    }
    void removeComment(int index) {
        if (index < 0 || index >= commentCount) return;
        string* temp = new string[commentCount - 1];
        for (int i = 0, j = 0; i < commentCount; ++i) {
            if (i != index) temp[j++] = comments[i];
        }
        --commentCount;
        delete[] comments;
        comments = temp;
    }
    void showComments() const {
        for (int i = 0; i < commentCount; ++i)
            cout << "    Comment " << i + 1 << ": " << comments[i] << endl;
    }
    int getLikes() const { return likes; }
    int getPostID() const { return postID; }
    string getContent() const { return content; }
    Post& operator++() { ++likes; return *this; }
};

int Post::nextPostID = 101;

class TextPost : public Post {
public:
    TextPost(string c) : Post(c) {}
    void display() const override {
        cout << "[TextPost #" << postID << "] " << content << " (Likes: " << likes << ")" << endl;
        showComments();
    }
    Post* clone() const override {
        return new TextPost(content);
    }
};

class ImagePost : public Post {
    string imageURL;
public:
    ImagePost(string c, string url) : Post(c), imageURL(url) {}
    void display() const override {
        cout << "[ImagePost #" << postID << "] " << content << " [Image: " << imageURL << "] (Likes: " << likes << ")" << endl;
        showComments();
    }
    string getImageURL() const { return imageURL; }
    Post* clone() const override {
        return new ImagePost(content, imageURL);
    }
};

class User {
    int userID;
    string name, bio;
    bool isPrivate;
    Post** posts;
    int postCount;
    User** following;
    int followingCount;
    User** followers;
    int followerCount;
    string* messages;
    int messageCount;
    string* stories;
    int storyCount;
    Post** favorites;
    int favCount;
    User** blockedUsers = new User * [0];
    int blockedCount = 0;
    static int nextID;
public:
    User(string n, bool priv, string b) : name(n), isPrivate(priv), bio(b), postCount(0), followingCount(0),followerCount(0), messageCount(0), storyCount(0), favCount(0) {
        userID = nextID++;
        posts = new Post * [0];
        following = new User * [0];
        followers = new User * [0];
        messages = new string[0];
        stories = new string[0];
        favorites = new Post * [0];
    }
    ~User() {
        for (int i = 0; i < postCount; ++i) delete posts[i];
        delete[] posts;
        delete[] following;
        delete[] followers;
        delete[] messages;
        delete[] stories;
        delete[] favorites;
        delete[] blockedUsers;
    }

    void addPost(Post* p) {
        Post** temp = new Post * [postCount + 1];
        for (int i = 0; i < postCount; i++) {
            temp[i] = posts[i];
        }
        temp[postCount++] = p;
        delete[] posts;
        posts = temp;
    }

    void blockUser(User* u) {
        for (int i = 0; i < blockedCount; ++i) {
            if (blockedUsers[i] == u) return;
        }
        User** temp = new User * [blockedCount + 1];
        for (int i = 0; i < blockedCount; ++i) temp[i] = blockedUsers[i];
        temp[blockedCount++] = u;
        delete[] blockedUsers;
        blockedUsers = temp;
        cout << "User " << u->getName() << " has been blocked.\n";
    }

    bool isBlocked(User* u) const {
        for (int i = 0; i < blockedCount; ++i)
            if (blockedUsers[i] == u) return true;
        return false;
    }

    void follow(User* u) {
        User** temp = new User * [followingCount + 1];
        for (int i = 0; i < followingCount; ++i) {
            temp[i] = following[i]; 
        }
        temp[followingCount++] = u;
        delete[] following;
        following = temp;

        User** temp2 = new User * [u->followerCount + 1];
        for (int i = 0; i < u->followerCount; ++i) {
            temp2[i] = u->followers[i];
        }
        temp2[u->followerCount++] = this;
        delete[] u->followers;
        u->followers = temp2;
    }

    void unfollow(User* u) {
        int idx = -1;
        for (int i = 0; i < followingCount; ++i) {
            if (following[i] == u) {
                idx = i; break;
            }
        }
        if (idx != -1) {
            User** temp = new User * [followingCount - 1];
            for (int i = 0, j = 0; i < followingCount; ++i) {
                if (i != idx) temp[j++] = following[i];
            }
            delete[] following;
            following = temp;
            --followingCount;
        }

        idx = -1;
        for (int i = 0; i < u->followerCount; ++i) {
            if (u->followers[i] == this) {
                idx = i; break;
            }
        }
        if (idx != -1) {
            User** temp = new User * [u->followerCount - 1];
            for (int i = 0, j = 0; i < u->followerCount; ++i) {
                if (i != idx) {
                    temp[j++] = u->followers[i];
                }
            }
            delete[] u->followers;
            u->followers = temp;
            --u->followerCount;
        }

        cout << "You have unfollowed " << u->getName() << ".";
    }

    void showProfile() const {
        cout << " User ProfileID: " << userID << "Name: " << name<< "Bio : " << bio<< "Privacy : " << (isPrivate ? "Private" : "Public") << "";
    }

    int getUserID() const {
        return userID; 
    }
    string getName() const {
        return name; 
    }

    Post* findPostByID(int pid) {
        for (int i = 0; i < postCount; ++i) {
            if (posts[i]->getPostID() == pid) {
                return posts[i];
            }
        }
        return nullptr;
    }

    void addStory(string s) {
        string* temp = new string[storyCount + 1];
        for (int i = 0; i < storyCount; ++i) {
            temp[i] = stories[i];
        }
        temp[storyCount++] = s;
        delete[] stories;
        stories = temp;
    }

    void showStories() const {
        if (storyCount == 0) {
            cout << "No stories available.";
        }
        else {
            cout << " Stories: ";
                for (int i = 0; i < storyCount; ++i)
                    cout << i + 1 << ". " << stories[i] << "";
        }
    }

    void sendMessage(string m) {
        string* temp = new string[messageCount + 1];
        for (int i = 0; i < messageCount; ++i) {
            temp[i] = messages[i];
        }
        temp[messageCount++] = m;
        delete[] messages;
        messages = temp;
    }

    void viewInbox() const {
        cout << " Inbox:";
        for (int i = 0; i < messageCount; ++i) {
            cout << i + 1 << ". " << messages[i] << "";
        }
    }

    void saveFavorite(Post* p) {
        Post** temp = new Post * [favCount + 1];
        for (int i = 0; i < favCount; ++i) {
            temp[i] = favorites[i];
        }
        temp[favCount++] = p;
        delete[] favorites;
        favorites = temp;
    }

    void viewFavorites() const {
        if (favCount == 0) {
            cout << "No favorites saved.";
        }
        else {
            cout << " Favorite Posts:";
                for (int i = 0; i < favCount; ++i)
                    favorites[i]->display();
        }
    }

    Post** getPosts() const {
        return posts;
    }
    int getPostCount() const {
        return postCount; 
    }
    bool isFriend(User* u) {
        for (int i = 0; i < followerCount; ++i) {
            if (followers[i] == u) {
                return true;
            }
        }
        return false;
    }

    void showPosts(User* viewer) {
        if (isPrivate && !isFriend(viewer)) {
            cout << name << "'s account is private.";
            return;
        }
        cout << " Posts by " << name << ":";
        for (int i = 0; i < postCount; ++i) {
            posts[i]->display();
        }
    }

    int totalLikes() const {
        int total = 0;
        for (int i = 0; i < postCount; ++i) {
            total += posts[i]->getLikes();
        }
        return total;
    }

    void removePost(int pid) {
        int index = -1;
        for (int i = 0; i < postCount; ++i) {
            if (posts[i]->getPostID() == pid) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            return;
        }
        delete posts[index];
        Post** temp = new Post * [postCount - 1];
        for (int i = 0, j = 0; i < postCount; ++i)
            if (i != index) temp[j++] = posts[i];
        delete[] posts;
        posts = temp;
        --postCount;
        cout << " Post removed successfully.";
    }
};

int User::nextID = 1;

int main() {
    User** users = new User * [0];
    int userCount = 0;
    int mainChoice, subChoice;
    int uid, pid, a, b, fromID, toID, postID, index, commenterID;
    string name, bio, content, url, msg, s, cmt;

    cout << "\n\t Welcome to GhostTag - The C++ Social Media App!\n";

    do {
        cout << "\n====== Main Menu ======\n";
        cout << "1. User Management\n";
        cout << "2. Post Management\n";
        cout << "3. Social Features\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> mainChoice;

        switch (mainChoice) {
        case 1: // User Management Submenu
            do {
                cout << "\n-- User Management --\n";
                cout << "1. Create User\n";
                cout << "2. Delete User\n";
                cout << "3. View All Users\n";
                cout << "4. Block User\n";
                cout << "5. Back\n";
                cout << "Enter choice: ";
                cin >> subChoice;

                switch (subChoice) {
                case 1: {
                    cout << "Enter name: ";
                    cin.ignore(); getline(cin, name);
                    cout << "Enter bio: ";
                    getline(cin, bio);
                    int priv;
                    cout << "Private account? (1 = yes, 0 = no): ";
                    cin >> priv;
                    User** temp = new User * [userCount + 1];
                    for (int i = 0; i < userCount; ++i) temp[i] = users[i];
                    temp[userCount++] = new User(name, priv, bio);
                    delete[] users;
                    users = temp;
                    break;
                }
                case 2: {
                    cout << "Enter User ID to delete: ";
                    cin >> uid;
                    int index = -1;
                    for (int i = 0; i < userCount; ++i)
                        if (users[i]->getUserID() == uid) { index = i; break; }
                    if (index != -1) {
                        delete users[index];
                        User** temp = new User * [userCount - 1];
                        for (int i = 0, j = 0; i < userCount; ++i)
                            if (i != index) temp[j++] = users[i];
                        delete[] users;
                        users = temp;
                        --userCount;
                        cout << "User deleted.\n";
                    }
                    else cout << "User not found.\n";
                    break;
                }
                case 3:
                    for (int i = 0; i < userCount; ++i)
                        users[i]->showProfile();
                    break;
                case 4: {
                    cout << "Your User ID: ";
                    cin >> a;
                    cout << "User ID to block: ";
                    cin >> b;
                    User* blocker = nullptr, * blockee = nullptr;
                    for (int i = 0; i < userCount; ++i) {
                        if (users[i]->getUserID() == a) blocker = users[i];
                        if (users[i]->getUserID() == b) blockee = users[i];
                    }
                    if (blocker && blockee) {
                        blocker->blockUser(blockee);
                        cout << blocker->getName() << " blocked " << blockee->getName() << ".\n";
                    }
                    else cout << "Invalid User IDs.\n";
                    break;
                }
                }
            } while (subChoice != 5);
            break;

        case 2: // Post Management Submenu
            do {
                cout << "\n-- Post Management --\n";
                cout << "1. Create Post\n";
                cout << "2. Remove Post\n";
                cout << "3. View Posts\n";
                cout << "4. Show Trending Post\n";
                cout << "5. Save as Favorite\n";
                cout << "6. View Favorites\n";
                cout << "7. Back\n";
                cout << "Enter choice: ";
                cin >> subChoice;

                switch (subChoice) {
                case 1: {
                    cout << "Available Users:\n";
                    for (int i = 0; i < userCount; ++i)
                        cout << users[i]->getUserID() << ": " << users[i]->getName() << "\n";
                    cout << "Enter User ID: ";
                    cin >> uid;
                    cin.ignore();
                    for (int i = 0; i < userCount; i++) {
                        if (users[i]->getUserID() == uid) {
                            cout << "1. TextPost, 2. ImagePost: ";
                            cin >> subChoice;
                            cin.ignore();
                            cout << "Enter content: ";
                            getline(cin, content);
                            if (subChoice == 1)
                                users[i]->addPost(new TextPost(content));
                            else {
                                cout << "Enter image URL: ";
                                getline(cin, url);
                                users[i]->addPost(new ImagePost(content, url));
                            }
                        }
                    }
                    break;
                }
                case 2:
                    cout << "Enter your ID: ";
                    cin >> uid;
                    cout << "Post ID to remove: ";
                    cin >> pid;
                    for (int i = 0; i < userCount; ++i)
                        if (users[i]->getUserID() == uid)
                            users[i]->removePost(pid);
                    break;
                case 3:
                    cout << "Enter your ID: ";
                    cin >> uid;
                    cout << "Enter profile User ID to view: ";
                    cin >> pid;
                    for (int i = 0; i < userCount; ++i) {
                        if (users[i]->getUserID() == uid && users[i]->isBlocked(users[pid - 1])) {
                            cout << "You are blocked from viewing this user's posts.\n";
                            break;
                        }
                        if (users[i]->getUserID() == pid)
                            users[i]->showPosts(users[uid - 1]);
                    }
                    break;
                case 4: {
                    Post* trending = nullptr;
                    for (int i = 0; i < userCount; ++i) {
                        Post** posts = users[i]->getPosts();
                        for (int j = 0; j < users[i]->getPostCount(); ++j) {
                            if (!trending || posts[j]->getLikes() > trending->getLikes())
                                trending = posts[j];
                        }
                    }
                    if (trending) {
                        cout << "\nTrending Post:\n";
                        trending->display();
                    }
                    else cout << "No posts yet.\n";
                    break;
                }
                case 5:
                    cout << "Your ID: ";
                    cin >> uid;
                    cout << "Post ID to save: ";
                    cin >> pid;
                    for (int i = 0; i < userCount; ++i)
                        if (users[i]->getUserID() == uid)
                            for (int j = 0; j < userCount; ++j)
                                if (Post* p = users[j]->findPostByID(pid))
                                    users[i]->saveFavorite(p);
                    break;
                case 6:
                    cout << "Enter User ID: ";
                    cin >> uid;
                    for (int i = 0; i < userCount; ++i)
                        if (users[i]->getUserID() == uid)
                            users[i]->viewFavorites();
                    break;
                }
            } while (subChoice != 7);
            break;

        case 3: 
            do {
                cout << "\n-- Social Features --\n";
                cout << "1. Follow User\n";
                cout << "2. Unfollow User\n";
                cout << "3. Like Post\n";
                cout << "4. Comment on Post\n";
                cout << "5. Remove Comment\n";
                cout << "6. Share Post\n";
                cout << "7. Add Story\n";
                cout << "8. View Stories\n";
                cout << "9. Send Message\n";
                cout << "10. View Inbox\n";
                cout << "11. Back\n";
                cout << "Enter choice: ";
                cin >> subChoice;

                switch (subChoice) {
                case 1: {
                    cout << "Your ID: "; cin >> a;
                    cout << "Follow User ID: "; cin >> b;
                    User* u1 = nullptr, * u2 = nullptr;
                    for (int i = 0; i < userCount; ++i) {
                        if (users[i]->getUserID() == a) u1 = users[i];
                        if (users[i]->getUserID() == b) u2 = users[i];
                    }
                    if (u1 && u2 && !u2->isBlocked(u1)) {
                        u1->follow(u2);
                        cout << u1->getName() << " followed " << u2->getName() << "!\n";
                    }
                    else {
                        cout << "Cannot follow. You may be blocked.\n";
                    }
                    break;
                }
                case 2: {
                    cout << "Your ID: "; cin >> a;
                    cout << "Unfollow User ID: "; cin >> b;
                    User* u1 = nullptr, * u2 = nullptr;
                    for (int i = 0; i < userCount; ++i) {
                        if (users[i]->getUserID() == a) u1 = users[i];
                        if (users[i]->getUserID() == b) u2 = users[i];
                    }
                    if (u1 && u2) u1->unfollow(u2);
                    break;
                }
                case 3: {
                    cout << "Enter your User ID: "; cin >> uid;
                    User* liker = nullptr;
                    for (int i = 0; i < userCount; ++i)
                        if (users[i]->getUserID() == uid) liker = users[i];
                    if (!liker) {
                        cout << "Invalid User ID.\n";
                    }
                    else {
                        cout << "Enter Post ID to like: ";
                        cin >> pid;
                        for (int i = 0; i < userCount; ++i)
                            if (Post* p = users[i]->findPostByID(pid)) {
                                ++(*p);
                                cout << liker->getName() << " liked the post!\n";
                                break;
                            }
                    }
                    break;
                }
                case 4: {
                    cout << "Enter your User ID: "; cin >> commenterID;
                    cout << "Enter Post ID to comment on: "; cin >> pid;
                    cin.ignore();
                    cout << "Enter your comment: ";
                    getline(cin, cmt);
                    User* commenter = nullptr;
                    for (int i = 0; i < userCount; ++i)
                        if (users[i]->getUserID() == commenterID) commenter = users[i];
                    if (!commenter) {
                        cout << "Invalid User ID.\n";
                        break;
                    }
                    for (int i = 0; i < userCount; ++i)
                        if (Post* p = users[i]->findPostByID(pid)) {
                            p->addComment(commenter->getName() + ": " + cmt);
                            cout << "Comment added by " << commenter->getName() << ".\n";
                            break;
                        }
                    break;
                }
                case 5: {
                    cout << "Enter your User ID: "; cin >> uid;
                    cout << "Enter Post ID: "; cin >> pid;
                    cout << "Comment number to remove: "; cin >> index;
                    for (int i = 0; i < userCount; ++i)
                        if (Post* p = users[i]->findPostByID(pid)) {
                            p->removeComment(index - 1);
                            cout << "Comment removed by user #" << uid << ".\n";
                            break;
                        }
                    break;
                }
                case 6: {
                    cout << "From User ID: "; cin >> fromID;
                    cout << "To User ID: "; cin >> toID;
                    cout << "Post ID to share: "; cin >> postID;
                    User* from = nullptr, * to = nullptr;
                    for (int i = 0; i < userCount; ++i) {
                        if (users[i]->getUserID() == fromID) from = users[i];
                        if (users[i]->getUserID() == toID) to = users[i];
                    }
                    if (from && to && !to->isBlocked(from)) {
                        Post* p = from->findPostByID(postID);
                        if (p) {
                            Post* shared = p->clone();
                            to->addPost(shared);
                            cout << "Post shared.\n";
                        }
                    }
                    else {
                        cout << "Cannot share post. You may be blocked.\n";
                    }
                    break;
                }
                case 7: {
                    cout << "Enter User ID: "; cin >> uid;
                    cin.ignore();
                    cout << "Enter story: "; getline(cin, s);
                    for (int i = 0; i < userCount; ++i)
                        if (users[i]->getUserID() == uid)
                            users[i]->addStory(s);
                    break;
                }
                case 8: {
                    cout << "Enter User ID: "; cin >> uid;
                    for (int i = 0; i < userCount; ++i)
                        if (users[i]->getUserID() == uid)
                            users[i]->showStories();
                    break;
                }
                case 9: {
                    cout << "Sender ID: "; cin >> fromID;
                    cout << "Receiver ID: "; cin >> toID;
                    cin.ignore();
                    cout << "Enter message: "; getline(cin, msg);
                    for (int i = 0; i < userCount; ++i)
                        if (users[i]->getUserID() == toID && !users[i]->isBlocked(users[fromID - 1]))
                            users[i]->sendMessage("From " + to_string(fromID) + ": " + msg);
                    break;
                }
                case 10: {
                    cout << "Enter User ID: "; cin >> uid;
                    for (int i = 0; i < userCount; ++i)
                        if (users[i]->getUserID() == uid)
                            users[i]->viewInbox();
                    break;
                }
                }
            } while (subChoice != 11);
            break;

        case 4:
            cout << "\n Thank you for using GhostTag. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice.\n";
        }

    } while (mainChoice != 4);

    for (int i = 0; i < userCount; ++i) delete users[i];
    delete[] users;

    return 0;
}
