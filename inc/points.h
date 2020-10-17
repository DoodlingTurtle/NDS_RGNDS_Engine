#ifndef __RGNDS_POINT_H__
#define __RGNDS_POINT_H__ 1

namespace RGNDS {

    template <typename T>
    struct Point {
        T x = 0, y = 0;

        template<typename A>
        Point<T>* operator=(const Point<A>& p) const {
            this.x = p.x;
            this.y = p.y;
            return this;
        }
        template<typename A>
        Point<T> operator+(const Point<A>& p) const {
            Point<T> o;
            o.x = this->x + p.x;
            o.y = this->y + p.y;
            return o;
        }
        template<typename A>
        Point<T> operator*(const Point<A>& p) const {
            Point<T> o;
            o.x = this->x * p.x;
            o.y = this->y * p.y;
            return o;
        }
        template<typename A>
        Point<T> operator-(const Point<A>& p) const {
            Point<T> o;
            o.x = this->x - p.x;
            o.y = this->y - p.y;
            return o;
        }

        template<typename A>
        Point<T>* operator+=(const Point<A>& p) {
            this->x += p.x;
            this->y += p.y;
            return this;
        }

        template<typename A>
        Point<T> operator*(const A& p) const {
            Point<T> o;
            o.x = this->x * p;
            o.y = this->y * p;
            return o;
        }


        template<typename A>
        Point<T>* operator*=(const Point<A>& p) {
            this->x *= p.x;
            this->y *= p.y;

            return this;
        }


        template<typename A>
        Point<T>* operator+=(const A& p) {
            this->x += p;
            this->y += p;
            return this;
        }
        template<typename A>
        Point<T>* operator*=(const A& p) {
            this->x *= p;
            this->y *= p;
            return this;
        }

        template<typename A>
        Point<T> operator-(const A& p) const {
            Point<T> o;
            o.x = this->x - p;
            o.y = this->y - p;
            return o;
        }

        template<typename A>
        Point<T> operator/(const A& p) const {
            Point<T> o;
            if(p == 0)
                return o;

            o.x = this->x / p;
            o.y = this->y / p;
            return o;
        }


        template<typename A>
        Point<A> to() const {
            Point<A> o;
            o.x = (A)this->x;
            o.y = (A)this->y;
            return o;
        }

        T dist() const {
            return sqrt(this->x*this->x + this->y*this->y);
        }

        Point<T> normalize() const {
            Point<T> o;
            T d = this->dist();
            o.x = this->x / d;
            o.y = this->y / d;
            return o;
        }
    };
}


#endif // __RGNDS_POINT_H__
