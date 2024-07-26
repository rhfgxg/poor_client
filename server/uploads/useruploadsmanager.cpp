#include "useruploadsmanager.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "../../feature/data/packet.h"  // 自定义数据包

UserUploadsManager::UserUploadsManager(ClientNetwork *network_, QObject *parent)  :
    QObject(parent),
    network(network_)
{
    // loginResponse：服务器通讯类的信号，在接收到服务器消息时发出，携带了收到的消息
    connect(network, &ClientNetwork::uploadsInitResponse, this, &UserUploadsManager::onReadyReadUploadsInit);
    // loginResponse：服务器通讯类的信号，在接收到服务器消息时发出，携带了收到的消息
    connect(network, &ClientNetwork::uploadsCompleteResponse, this, &UserUploadsManager::onReadyReadUploadsComplete);
}

UserUploadsManager::~UserUploadsManager()
{

}

// 初始化文件上传任务，分配文件id，创建数据库数据，准备接收文件
void UserUploadsManager::sendInitialUploadRequest(const QString& file_path)
{
    QFile file(file_path);  // 文件路径
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件上传任务初始化：文件打开失败: " << file_path;
        return;
    }

    QString file_name = QFileInfo(file_path).fileName();    // 文件原始名
    QString account = "123"; // 用户账号，保存在本地文件
    QString client_id = network->clientId(); // 客户端ID
    qint64 file_size = file.size(); // 文件总大小
    QJsonArray missing_parts;   // json数组，保存缺失列表：位图
    {
        const qint64 chunk_size = 1024 * 1024; // 每个切片1MB
        int total_blocks = (file_size + chunk_size - 1) / chunk_size;    // 计算文件切片数量

        QVector<bool> bitmap(total_blocks, false); // 客户端位图，false表示未上传
        for (bool bit : bitmap)
        {
            missing_parts.append(bit);
        }
    }

    QString file_format = QFileInfo(file_path).suffix().toLower();    // 查看文件扩展名
//    QStringList allowedExtensions = {"jpg", "png", "txt", "pdf"};
//    if (!allowedExtensions.contains(file_format))
//    {// 禁止上传的文件格式
//        qDebug() << "不允许的文件扩展名:" << fileExtension;
//        return;
//    }

    // json子数据包
    QJsonObject request;
    request["file_name"] = file_name;    // 客户端展示文件名
    request["file_format"] = file_format;   // 文件拓展名
    request["file_size"] = file_size;  // 总大小
    request["account"] = account;    // 用户账号
    request["client_id"] = client_id;    // 客户端ID
    request["file_path"] = file_path;   // 客户端源文件路径，通过服务器间接传递给具体执行上传的函数
    request["missing_parts"] = missing_parts;    // 文件缺失列表：位图

    // 使用数据头和json数据包，打包进自定义数据包
    Packet message(PacketType::INITIAL_UPLOAD, request);
    network->sendMessage(message.toByteArray());  // 序列化后，由通信管理发送给服务端
}

// 文件切片上传
void UserUploadsManager::uploadFileInChunks(const QString file_id, const QString& file_path, QVector<bool> missing_parts)
{/* 参数：文件ID，文件路径，偏移量列表
  * 遍历偏移量列表，进行上传
  * 遍历结束后，上传数据携带 上传完成 的结果，要求服务端返回缺失的列表
  * 然后再次遍历新的列表，直到服务器返回状态 上传完成
  * 进度条：文件上传完第一轮后，进度条显示80%，然后根据缺失的数量，显示余下的进度条
  * 完整上传后，进度条显示99%，进行余下的收尾工作
  */
    QFile file(file_path);  // 文件路径
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件上传：文件打开失败: " << file_path;
        return;
    }

    const qint64 chunk_size = 1024 * 1024; // 每个切片1MB

    // 上传的数据
    Packet request;
    request.setType(PacketType::UPLOAD_CHUNK);   // 设置数据包类型
    QByteArray file_data;       // 上传的 文件数据子包
    QJsonObject request_json;   // 上传的 json子包
    request_json["file_id"] = file_id; // 文件id

    // 遍历缺失列表，然后上传文件块
    for (int i = 0; i < missing_parts.size(); ++i)
    {
        if (!missing_parts[i]) // 如果此下标为false，表示对应位置未上传
        {
            qint64 offset = i * chunk_size; // 获得偏移量
            if (!file.seek(offset))
            {
                qDebug() << "文件上传：文件定位失败: " << file_path;
                file.close();
                return;
            }
            file_data = file.read(chunk_size);  // 每次读取一片的大小
            request.setFileData(file_data); // 将文件块加入数据包

            request_json["offset"] = offset; // 文件偏移地址（文件块下标）
            request.setJsonData(request_json);  // 添加json子数据包

            // 序列化数据，发送数据给服务器
            network->sendMessage(request.toByteArray());
        }
    }

    // 如果遍历结束，请求返回上传结果
    Packet complete_request;
    complete_request.setType(PacketType::UPLOAD_COMPLETE);  // 设置数据包类型：请求返回上传状态

    QJsonObject complete_json;
    complete_json["file_id"] = file_id;

    complete_request.setJsonData(complete_json);
    network->sendMessage(complete_request.toByteArray());

    file.close();
}

// 接受服务器发回数据：发回的文件id
void UserUploadsManager::onReadyReadUploadsInit(const QJsonObject &request)
{
    QString status = request["status"].toString();

    if (status == "SUCCESS")
    {
        qDebug("文件上传任务初始化成功");

        QString file_id  = request["file_id"].toString();   // 文件ID
        QString file_path = request["file_path"].toString();// 文件客户端保存地址
        QVector<bool> missing_parts;   // 缺失列表：位图;
        {
            QJsonArray json_array = request["missing_parts"].toArray();

            // 转换为 QVector<bool>
            for (const QJsonValue &value : json_array)
            {
                missing_parts.append(value.toBool());
            }
        }
        uploadFileInChunks(file_id, file_path, missing_parts);   // 初始化成功：开始执行上传任务

        // todo：将任务加入任务列表（配置文件）
    }
    else
    {
        qDebug() << "文件上传任务初始化失败";
        // 清除任务，清楚文件资源等
    }
}

// 接收服务端响应数据：文件是否完整上传
void UserUploadsManager::onReadyReadUploadsComplete(const QJsonObject &request)
{
    QString status = request["status"].toString();  // 结果

    if (status == "SUCCESS")
    {/*文件上传成功*/
        qDebug("文件完整上传");
        // todo：将任务从任务列表（配置文件）移除
        // 显示结果
    }
    else
    {/*文件上传失败：查询异常，未上传完成*/
        if (request["message"].toString() == "文件未上传完成")
        {/* 文件未上传完成：再次遍历缺失列表上传*/
            QString file_id = request["file_id"].toString();    // 文件ID
            QString file_path = request["file_path"].toString();// 文件客户端保存地址

            QVector<bool> missing_parts;   // 缺失列表：位图;
            {
                QJsonArray json_array = request["missing_parts"].toArray();    // 从json数组 转回QJsonArray

                for (const QJsonValue &value : json_array) // 遍历，将每一个元素转为bool，存入vector
                {
                    missing_parts.append(value.toBool());
                }
            }

            qDebug("文件未上传成功");
            uploadFileInChunks(file_id, file_path, missing_parts);   // 初始化成功：开始执行上传任务
        }
        else
        {
            // 查询失败：重新发起查询请求
            Packet complete_request;
            complete_request.setType(PacketType::UPLOAD_COMPLETE);  // 设置数据包类型：请求返回上传状态

            QJsonObject complete_json;
            complete_json["file_id"] = request["file_id"].toString();    // 文件ID

            complete_request.setJsonData(complete_json);
            network->sendMessage(complete_request.toByteArray());
        }
    }
}
