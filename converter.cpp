#include "converter.h"

Converter::Converter(QObject *parent)
    : QObject{parent}
{

}

//recebe um ponteiro de Test e um path de arquivo
void Converter::writeJson(Test *obj, QString path)
{
    //cria dois wrappers de map
    QVariantMap map;
    QVariantMap items;
    //cria uma propriedade no map chamada "name" e associa ao nome do obj de Test
    map["name"] = obj->name();
    //1 - para cada key do retorno da função map (que retorna um mapa) do obj
    foreach(QString key, obj->map().keys())
    {
        //2 - adiciona esses valores e keys no QVariantMap items.
        //passando cada valor relacionado as keys do map como QVariant para items.
        //cada valor desse QVariantMap será um QVariant.
        items.insert(key, QVariant(obj->map().value(key)));
    }
    //passando o QVariantMap para QVariant e adicionando à propriedade items do map.
    map["items"] = QVariant(items);

    //pega o QVariantMap map e transforma em um doc JSON.
    QJsonDocument document = QJsonDocument::fromVariant(map);
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        qCritical() << "Could not write file!";
        qCritical() << file.errorString();
        return;
    }

    //converte um QJsonDocument em um documento JSON encodado utf-8, com o formato especificado.
    file.write(document.toJson());
    file.close();
}

//vai retornar um ponteiro
Test *Converter::readJson(QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qCritical() << "Could not read file!";
        qCritical() << file.errorString();
        return nullptr;
    }

    //vai ler tudo do arquivo e retornar em um QByteArray.
    QByteArray data = file.readAll();
    file.close();

    //converte o QByteArray novamente em um QJsonDocument
    QJsonDocument document = QJsonDocument::fromJson(data);
    //criado um ponteiro para um obj de Test na heap
    Test *t = new Test();
    //converte o valor da propriedade name do document e seta na func de Test (recebe uma QString)
    t->setName(document["name"].toString());
    //transforma a propriedade items do document (QJsonDocument) novamente em um QVariantMap
    QVariantMap vmap = qvariant_cast<QVariantMap>(document["items"]);

    //cria um QMap (estado original antes da serialização)
    QMap<QString, QString> map;
    foreach(QString key, vmap.keys())
    {
        //preenche o QMap com os itens do QVariantMap
        map.insert(key, vmap[key].toString());
    }
    //terminado a desserialização
    t->setMap(map);

    return t;
}
