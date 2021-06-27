#pragma once

class GLObject {
public:
    GLObject();
    unsigned get_id() const;
    const static unsigned ID_NONE;
protected:
    unsigned id;
};