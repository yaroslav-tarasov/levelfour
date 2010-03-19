// Toolbox with helper classes
#define _USE_MATH_DEFINES 1
#include <malloc.h>
#include <math.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <time.h>
#include <QtCore/QList>
#include <Key.h>

#ifndef HELPER_H
#define HELPER_H


class Helpers
{
public:
    class InterpolationH
    {
    public:
        static inline float Linear(float y1, float y2, unsigned int pos, unsigned int steps)
        {
            register float mu = (float) pos / (float) steps;
            return y1*(1.0-mu) + y2*mu;
        }

        static inline float Linear(float y1, float y2, float mu)
        {
            return y1*(1.0-mu) + y2*mu;
        }

        static inline float Cosine(float y1, float y2, unsigned int pos, unsigned int steps)
        {
            register float mu = (float) pos / (float) steps;
            register float mu2 = (1-cos(mu*M_PI))/2;
            return (y1*(1-mu2)+y2*mu2);
        }

        static inline float Cosine(float y1, float y2, float mu)
        {
            register float mu2 = (1-cos(mu*M_PI))/2;
            return (y1*(1-mu2)+y2*mu2);
        }

        static inline float Cubic(float y1, float y2, float y3, float y4, float pos)
        {
            register float p = (y4-y3) - (y1-y2);
            register float q = (y1-y2) - p;
            register float r = y3 - y1;
            register float s = y2;

            return p*pow(pos, 3.0f) + q*pow(pos, 2.0f) + r*pos + s;
        }

        static inline void InterpolTo(std::vector<float> &v, unsigned int length)
        {
            unsigned int i_pos1, i_pos2;
            register float scale, pos;

            i_pos1 = i_pos2 = 0;
            scale  = (float) (v.size()-1) / (length-1);
            std::vector<float> newdata(length);
            for (unsigned int i=0; i<length-1; ++i)
            {
                i_pos2    =    ceil(i * scale);
                if (i_pos2 == i_pos1+2)
                    i_pos1 = i_pos2-1;
                pos        =        (i * scale) - (i_pos1);
                newdata[i] = Helpers::InterpolationH::Linear(v[i_pos1], v[i_pos2], pos);
            }
            newdata[newdata.size()-1] = v[v.size()-1];
            v = newdata;
        }
    };

    class AnalysisH
    {
    public:
        template <class T>
        static inline std::vector<T> ComputeDistance(std::vector<T> &v1, std::vector<T> &v2)
        {
            int i_pos1, i_pos2;
            register T pos, newdata, scale;
            i_pos1 = i_pos2 = 0;
            const unsigned int v1_size = v1.size();
            const unsigned int v2_size = v2.size();

            if (v1_size == v2_size)
            {
                std::vector<T> returnvalue(v1_size);
                for (unsigned int i = 0; i < v1_size; ++i)
                {
                    returnvalue[i] = abs(v1[i] - v2[i]);
                }
                return returnvalue;
            }
            else if (v1_size > v2_size)
            {
                scale = ((T) v2_size-1) / (v1_size-1);
                std::vector<T> returnvalue(v1_size);
                for (unsigned int i = 0; i < v1_size-1; ++i)
                {
                    i_pos2            =    ceil(i * scale);
                    if (i_pos2 == i_pos1+2)
                        i_pos1        = i_pos2-1;
                    pos                =        (i * scale) - i_pos1;
                    newdata            = Helpers::InterpolationH::Linear(v2[i_pos1], v2[i_pos2], pos);
                    returnvalue[i]    = abs(v1[i]-newdata);
                }
                returnvalue[returnvalue.size()-1] = abs(v1[v1_size-1] - v2[v2_size-1]);
                return returnvalue;
            }
            else
            {
                scale = ((T) v1_size-1) / (v2_size-1);
                std::vector<T> returnvalue(v2_size);
                for (unsigned int i = 0; i < v2_size-1; ++i)
                {
                    i_pos2            =    ceil(i * scale);
                    if (i_pos2 == i_pos1+2)
                        i_pos1        = i_pos2-1;
                    pos                =        (i * scale) - i_pos1;
                    newdata            = Helpers::InterpolationH::Linear(v1[i_pos1], v1[i_pos2], pos);
                    returnvalue[i]    = abs(v2[i]-newdata);
                }
                returnvalue[returnvalue.size()-1] = abs(v2[v2_size-1] - v1[v1_size-1]);
                return returnvalue;
            }
        }

        static inline std::vector<float> SummVerctors(const std::vector<float> &v1, const std::vector<float> &v2, float div = 1)
        {
            int i_pos1, i_pos2;
            register float pos, newdata, scale;
            i_pos1 = i_pos2 = 0;
            const unsigned int v1_size = (unsigned int) v1.size();
            const unsigned int v2_size = (unsigned int) v2.size();

            if (v2_size==0)
                return v1;

            if (v1_size == v2_size)
            {
                std::vector<float> returnvalue(v1_size);
                for (unsigned int i = 0; i < v1_size; ++i)
                    returnvalue[i] = (v1[i] + v2[i])/div;
                return returnvalue;
            }
            else if (v1_size > v2_size)
            {
                scale = ((float) v2_size-1) / (v1_size-1);
                std::vector<float> returnvalue(v1_size);
                for (unsigned int i = 0; i < v1_size-1; ++i)
                {
                    i_pos2            = ceil(i * scale);
                    if (i_pos2 == i_pos1+2)
                        i_pos1        = i_pos2-1;
                    pos                = (i * scale) - i_pos1;
                    newdata            = Helpers::InterpolationH::Linear(v2[i_pos1], v2[i_pos2], pos);
                    returnvalue[i]    = (v1[i] + newdata)/div;
                }
                returnvalue[returnvalue.size()-1] = (v1[v1_size-1] + v2[v2_size-1])/div;
                return returnvalue;
            }
            else
            {
                scale = ((float) v1_size-1) / (v2_size-1);
                std::vector<float> returnvalue(v2_size);
                for (unsigned int i = 0; i < v2_size-1; ++i)
                {
                    i_pos2            = ceil(i * scale);
                    if (i_pos2 == i_pos1+2)
                        i_pos1        = i_pos2-1;
                    pos                = (i * scale) - i_pos1;
                    newdata            = Helpers::InterpolationH::Linear(v1[i_pos1], v1[i_pos2], pos);
                    returnvalue[i]    = (v2[i] + newdata)/div;
                }
                returnvalue[returnvalue.size()-1] = (v2[v2_size-1] + v1[v1_size-1])/div;
                return returnvalue;
            }
        }

        template <class T>
        static inline T MaxVectorAbs(const std::vector<T> &v)
        {
            T max, ab;
            ab = abs(*std::min_element(v.begin(), v.end()));
            max = abs(*std::max_element(v.begin(), v.end()));
            if (ab>max)
                return ab;
            else
                return max;
        }

        static inline void UniformVector(std::vector<float> &v)
        {
            register float offset, max;
            offset = v[0];
            max = MaxVectorAbs(v);

            if (max == 0) max = 1;
            v[0]=0;
            for (unsigned int i=1;i<v.size();++i)
            {
                v[i] = (v[i] - offset) / max;
            }
        }

        static inline void UniformVector(std::vector<float> &v, float max)
        {
            register float offset;
            offset = v[0];

            if (max == 0) max = 1;
            v[0]=0;
            for (unsigned int i=1;i<v.size();++i)
            {
                v[i] = (v[i] - offset) / max;
            }
        }

        template <class T>
        static inline void NormalizeVector(std::vector<T> &v, float max)
        {
            if (max == 0.0f) max = 1;

            for (unsigned int i=0;i<v.size();++i)
                v[i] /= max;
        }

        template <class T>
        static inline void NormalizeVector(std::vector<T> &v)
        {
            T max = MaxVectorAbs(v);
            if (max == 0.0f) max = 1;

            for (unsigned int i=0;i<v.size();++i)
                v[i] /= max;
        }

        static inline float SpreadVector(std::vector<float> &v)
        {
            if (v.size()==1)
                return v[0];
            register float mean = SummVector(v);
            register float returnvalue = 0.0f;
            for (unsigned int i=1;i<v.size();++i)
                returnvalue += pow((abs(v[i])-mean), 2.0f);
            return sqrt(returnvalue/(v.size()-1));
        }

		static inline float DistVector(const QList<Key *> *l1, const QList<float> *l2, const float &length = 1.0f)
        {
            register float returnvalue = 0.0f;
            for (int i=0; i<(*l1).size(); ++i) {
                returnvalue += pow( (*l1)[i]->getValue().toDouble() - (*l2)[i] / length, 2);
            }
            return 2.0f - sqrt(returnvalue);
        }

		static inline float DistVector(const QList<float> *l1, const QList<float> *l2, const float &length = 1.0f)
        {
            register float returnvalue = 0.0f;
            for (int i=0; i<(*l1).size(); ++i) {
                returnvalue += pow( (*l1)[i] - (*l2)[i] / length, 2);
            }
            return /*2.0f -*/ sqrt(returnvalue);
        }

        // if l1 == l2 returnvalue = 1
        static inline float AngleVector(const QList<Key *> *l1, const QList<float> *l2, const float &length = 1.0f)
        {
            register float returnvalue = 0.0f;
            if (length > 0) {
                for (int i=0; i<(*l1).size(); ++i)
                    returnvalue += (*l1)[i]->getValue().toDouble() * ((*l2)[i] / length);
            }
            return (returnvalue/2.0f) + 0.5f;
        }

		//if l1 == l2 returnvalue = 1
		/*static inline float AngleVector(const QList<Key *> *l1, const QList<float> *l2, const float &length)
        {
            register float returnvalue = 0.0f;
            if (length > 0) {
				for (int i=0; i<(*l1).size(); ++i) {
					register const float keyValue = (*l1)[i]->getValue().toDouble();
					register const float auValue  = (*l2)[i] / length;
					if (!sameSign(keyValue, auValue))
						continue;
                    returnvalue += keyValue * auValue;
				}
                returnvalue = (returnvalue/2.0f) + 0.5f;
            }
            return returnvalue;
        }*/

		static inline float DiceCoefficient(const QList<Key *> *l1, const QList<float> *l2, const float &length)
		{
			register float returnvalue, div;
			returnvalue = div = 0.0f;
			for (int i=0; i<(*l1).size(); ++i) {
				div += (*l1)[i]->getValue().toDouble() + (*l2)[i] / length;
				returnvalue += (*l1)[i]->getValue().toDouble() * ((*l2)[i] / length);
			}
			return 2.0f*returnvalue/div;
		}

        static inline float LengthVector(QList<float> *l)
        {
            register float returnvalue = 0.0f;
            foreach (float elem, *l)
                returnvalue += pow(elem, 2.0f);
            return sqrt(returnvalue);
        }

        static inline void NormalizeVectorLength(QList<float> *l)
        {
            register float length = LengthVector(l);
            if (length == 0.0f)
                return;

            for (int i = 0; i < l->size(); ++i)
                (*l)[i] = (*l)[i] / length;
        }

        static inline void NormalizeVectorLength(QList<float> *l, float &length)
        {
            if (length == 0.0f)
                return;
            for (int i = 0; i < l->size(); ++i)
                (*l)[i] = (*l)[i] / length;
        }

        static inline float LengthVector(QList<Key *> *l)
        {
            register float returnvalue = 0.0f;
            foreach (Key *elem, *l)
                returnvalue += pow(elem->getValue().toDouble(), 2);
            return sqrt(returnvalue);
        }

        static inline void NormalizeVectorLength(QList<Key *> *l)
        {
            register float length = LengthVector(l);
            if (length == 0.0)
                return;

            foreach (Key *key, *l)
                key->setValue(key->getValue().toDouble() / length);
        }

        static inline void NormalizeVectorLength(QList<Key *> *l, float &length)
        {
            foreach (Key *key, *l)
                key->setValue(key->getValue().toDouble() / length);
        }

        static inline float SummVector(std::vector<float> &v)
        {
            register float returnvalue = v[0];
            for (unsigned int i=1;i<v.size();++i)
                returnvalue += abs(v[i]);
            return returnvalue/v.size();
        }

		template<class T>
		static inline bool sameSign(T &a, T &b)
		{
			return (a < 0) == (b < 0);
		}
    };

    class StringH
    {
    public:
        static inline std::string FloatToString(const float &input)
        {
            std::stringstream str;
            str << input;

            return str.str();
        }

        static inline int Find_Chr(const std::string &ref, const std::string &search, int nbr, const int pos)
        {
            if (nbr == 0)
                return pos;
            else
            {
                --nbr;
                return Find_Chr(ref, search, nbr, (int) ref.find(search, pos+1));
            }
        }

        static inline int FindNbrChrs(std::string str, std::string set)
        {
            register int returnvalue = 0;
            for (unsigned int i=0; i<str.length(); ++i)
            {
                if (std::binary_search(set.begin(), set.end(), str[i]))
                    ++returnvalue;
            }
            return returnvalue;
        }
    };

    class MathH
    {
    public:
        static inline int GetSign(float const &n)
        {
            if        (n>0)    return  1;
            else            return -1;
        }

        static inline int NextPow2(int Number)
        {
            unsigned int RetVal = 1;
            __asm
            {
                xor ecx, ecx
                bsr ecx, Number
                inc ecx
                shl RetVal, cl
            }
            return(RetVal);
        }

        static inline int DoubleToInt(double d)
        {
          return d<0?d-.5:d+.5;
        }

        static inline double Round(double Zahl, int Stellen)
        {
            double v[] = { 1, 10, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8 };
            return floor(Zahl * v[Stellen] + 0.5) / v[Stellen];
        }
    };
    class TimeH
    {
    public:
        static inline char* GetSysTime()
        {
            time_t rawtime;
            time ( &rawtime );

            return ctime(&rawtime);
        }
    };
};

#endif