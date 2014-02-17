#pragma once

#include <QListView>

class UserphraseView final : public QListView {
    Q_OBJECT

public:
    explicit UserphraseView(QWidget *parent = 0);
    UserphraseView(const UserphraseView&) = delete;
    UserphraseView& operator=(const UserphraseView&) = delete;
    virtual ~UserphraseView() = default;

private:
};
