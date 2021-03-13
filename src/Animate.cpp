//
// Created by sun on 9/1/16.
//

#include "Animate.h"

Animate::Animate(QGraphicsItem *item, QGraphicsScene *scene)
        : type(0),
          m_duration(0), m_speed(0),
          toPos(), toScale(0), toOpacity(0),
          m_shape(QTimeLine::EaseInOutCurve),
          item(item), scene(scene)
{}

Animate &Animate::move(QPointF toPos)
{
    type |= MOVE;
    this->toPos = toPos;
    return *this;
}

Animate &Animate::scale(qreal toScale)
{
    type |= SCALE;
    this->toScale = toScale;
    return *this;
}

Animate &Animate::fade(qreal toOpacity)
{
    type |= FADE;
    this->toOpacity = toOpacity;
    return *this;
}

Animate &Animate::replace()
{
    type |= REPLACE;
    return *this;
}

Animate &Animate::shape(QTimeLine::CurveShape shape)
{
    m_shape = shape;
    return *this;
}

Animate &Animate::speed(qreal speed)
{
    m_speed = speed;
    return *this;
}

Animate &Animate::duration(int duration)
{
    m_duration = duration;
    return *this;
}

Animate &Animate::finish(std::function<void(void)> functor)
{
    finish([functor](bool) { functor(); });
    return *this;
}

Animate &Animate::finish(std::function<void(bool)> functor)
{
    Animation *animation = getAnimation(item);
    if (!(type & (MOVE | SCALE | FADE))) {
        if (animation) {
            animation->anim->stop();
            delete animation->anim;
            for (auto &keyFrame: animation->frames)
                keyFrame.finished(false);
            delete animation;
            setAnimation(item, nullptr);
        }
    }
    else if (animation && !(type & REPLACE))
        animation->frames.push_back({ type, m_duration, m_speed, toPos, toScale, toOpacity, functor, m_shape});
    else {
        if (animation) {
            animation->anim->stop();
            delete animation->anim;
            for (auto &keyFrame: animation->frames)
                keyFrame.finished(false);
            if (!(type & MOVE))
                for (auto &keyFrame: animation->frames)
                    if (keyFrame.type & MOVE) {
                        type |= MOVE;
                        toPos = keyFrame.toPos;
                    }
            if (!(type & SCALE))
                for (auto &keyFrame: animation->frames)
                    if (keyFrame.type & SCALE) {
                        type |= SCALE;
                        toScale = keyFrame.toScale;
                    }
            if (!(type & FADE))
                for (auto &keyFrame: animation->frames)
                    if (keyFrame.type & FADE) {
                        type |= FADE;
                        toOpacity = keyFrame.toOpacity;
                    }
            delete animation;
        }
        animation = new Animation;
        animation->scene = scene;
        animation->frames.push_back({type, m_duration, m_speed, toPos, toScale, toOpacity, functor, m_shape});
        setAnimation(item, animation);
        generateAnimation(item);
    }


    type = 0;
    m_duration = 0;
    m_speed = 0;
    toPos = QPointF();
    toScale = toOpacity = 0.0;
    m_shape = QTimeLine::EaseInOutCurve;
    return *this;
}

Animate::Animation *Animate::getAnimation(QGraphicsItem *item)
{
    if (!item->data(AnimationKey).isNull()) {
        auto prevAnim = static_cast<Animation *>(item->data(AnimationKey).value<void *>());
        return prevAnim;
    }
    return nullptr;
}

void Animate::setAnimation(QGraphicsItem *item, Animate::Animation *animation)
{
    item->setData(AnimationKey, QVariant::fromValue(static_cast<void *>(animation)));
}

void Animate::generateAnimation(QGraphicsItem *item)
{
    Animation *animation = getAnimation(item);
    KeyFrame &keyFrame = animation->frames.first();
    QPointF fromPos = item->pos(), toPos = keyFrame.toPos;
    qreal fromScale = item->scale(), toScale = keyFrame.toScale,
            fromOpacity = item->opacity(), toOpacity = keyFrame.toOpacity;
    bool move = static_cast<bool>(keyFrame.type & MOVE),
            scale = static_cast<bool>(keyFrame.type & SCALE),
            fade = static_cast<bool>(keyFrame.type & FADE);
    if (!keyFrame.duration) {
        QPointF posVec = toPos - fromPos;
        keyFrame.duration = qRound(qSqrt(QPointF::dotProduct(posVec, posVec)) / keyFrame.speed);
    }

    if (keyFrame.duration <= 0) {
        animation->frames.first().finished(true);
        animation->frames.pop_front();
        if (!animation->frames.isEmpty()) {
            generateAnimation(item);
        }
        else {
            delete animation;
            setAnimation(item, nullptr);
        }
    }
    else {
        animation->anim = new QTimeLine(keyFrame.duration, animation->scene);
        animation->anim->setUpdateInterval(20);
        animation->anim->setCurveShape(keyFrame.shape);
        QObject::connect(animation->anim, &QTimeLine::valueChanged, [item, fromPos, toPos, fromScale, toScale, fromOpacity, toOpacity, move, scale, fade](qreal x) {
            if (move)
                item->setPos((toPos - fromPos) * x + fromPos);
            if (scale)
                item->setScale((toScale - fromScale) * x + fromScale);
            if (fade)
                item->setOpacity((toOpacity - fromOpacity) * x + fromOpacity);
        });
        QObject::connect(animation->anim, &QTimeLine::finished, [item, animation] {
            animation->frames.first().finished(true);
            animation->frames.pop_front();
            if (!animation->frames.isEmpty()) {
                generateAnimation(item);
            }
            else {
                delete animation->anim;
                delete animation;
                setAnimation(item, nullptr);
            }
        });
        animation->anim->start();
    }
}

const int Animate::AnimationKey = 0;
