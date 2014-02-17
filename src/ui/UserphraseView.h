#pragma once

#include <QListView>

class UserphraseView final : public QListView {
    Q_OBJECT

public:
    explicit UserphraseView(QWidget *parent = 0) :QListView(parent) {}
    UserphraseView(const UserphraseView&) = delete;
    UserphraseView& operator=(const UserphraseView&) = delete;
    virtual ~UserphraseView() = default;

public slots:
    void remove();
};
